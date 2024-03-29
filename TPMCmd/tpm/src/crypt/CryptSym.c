//** Introduction
//
// This file contains the implementation of the symmetric block cipher modes
// allowed for a TPM. These functions only use the single block encryption functions
// of the selected symmetric crypto library.

//** Includes, Defines, and Typedefs
#include "Tpm.h"

#include "CryptSym.h"

#define KEY_BLOCK_SIZES(ALG, alg)                                    \
    static const INT16 alg##KeyBlockSizes[] = {ALG##_KEY_SIZES_BITS, \
                                               -1,                   \
                                               ALG##_BLOCK_SIZES};

FOR_EACH_SYM(KEY_BLOCK_SIZES)

//** Initialization and Data Access Functions
//
//*** CryptSymInit()
// This function is called to do _TPM_Init processing
BOOL CryptSymInit(void)
{
    return TRUE;
}

//*** CryptSymStartup()
// This function is called to do TPM2_Startup() processing
BOOL CryptSymStartup(void)
{
    return TRUE;
}

//*** CryptGetSymmetricBlockSize()
// This function returns the block size of the algorithm. The table of bit sizes has
// an entry for each allowed key size. The entry for a key size is 0 if the TPM does
// not implement that key size. The key size table is delimited with a negative number
// (-1). After the delimiter is a list of block sizes with each entry corresponding
// to the key bit size. For most symmetric algorithms, the block size is the same
// regardless of the key size but this arrangement allows them to be different.
//  Return Type: INT16
//   <= 0     cipher not supported
//   > 0      the cipher block size in bytes
LIB_EXPORT INT16 CryptGetSymmetricBlockSize(
    TPM_ALG_ID symmetricAlg,  // IN: the symmetric algorithm
    UINT16     keySizeInBits  // IN: the key size
)
{
    const INT16* sizes;
    INT16        i;
#define ALG_CASE(SYM, sym)          \
    case TPM_ALG_##SYM:             \
        sizes = sym##KeyBlockSizes; \
        break
    switch(symmetricAlg)
    {
#define GET_KEY_BLOCK_POINTER(SYM, sym) \
    case TPM_ALG_##SYM:                 \
        sizes = sym##KeyBlockSizes;     \
        break;
        // Get the pointer to the block size array
        FOR_EACH_SYM(GET_KEY_BLOCK_POINTER);

        default:
            return 0;
    }
    // Find the index of the indicated keySizeInBits
    for(i = 0; *sizes >= 0; i++, sizes++)
    {
        if(*sizes == keySizeInBits)
            break;
    }
    // If sizes is pointing at the end of the list of key sizes, then the desired
    // key size was not found so set the block size to zero.
    if(*sizes++ < 0)
        return 0;
    // Advance until the end of the list is found
    while(*sizes++ >= 0)
        ;
    // sizes is pointing to the first entry in the list of block sizes. Use the
    // ith index to find the block size for the corresponding key size.
    return sizes[i];
}

//** Symmetric Encryption
// This function performs symmetric encryption based on the mode.
//  Return Type: TPM_RC
//      TPM_RC_SIZE         'dSize' is not a multiple of the block size for an
//                          algorithm that requires it
//      TPM_RC_FAILURE      Fatal error
LIB_EXPORT TPM_RC CryptSymmetricEncrypt(
    BYTE*       dOut,           // OUT:
    TPM_ALG_ID  algorithm,      // IN: the symmetric algorithm
    UINT16      keySizeInBits,  // IN: key size in bits
    const BYTE* key,            // IN: key buffer. The size of this buffer
                                //     in bytes is (keySizeInBits + 7) / 8
    TPM2B_IV*  ivInOut,         // IN/OUT: IV for decryption.
    TPM_ALG_ID mode,            // IN: Mode to use
    INT32      dSize,           // IN: data size (may need to be a
                                //     multiple of the blockSize)
    const BYTE* dIn             // IN: data buffer
)
{
    BYTE*                   pIv;
    int                     i;
    BYTE                    tmp[MAX_SYM_BLOCK_SIZE];
    BYTE*                   pT;
    tpmCryptKeySchedule_t   keySchedule;
    INT16                   blockSize;
    TpmCryptSetSymKeyCall_t encrypt;
    BYTE*                   iv;
    BYTE                    defaultIv[MAX_SYM_BLOCK_SIZE] = {0};
    //
    pAssert(dOut != NULL && key != NULL && dIn != NULL);
    if(dSize == 0)
        return TPM_RC_SUCCESS;

    TPM_DO_SELF_TEST(algorithm);
    blockSize = CryptGetSymmetricBlockSize(algorithm, keySizeInBits);
    if(blockSize == 0)
        return TPM_RC_FAILURE;
    // If the iv is provided, then it is expected to be block sized. In some cases,
    // the caller is providing an array of 0's that is equal to [MAX_SYM_BLOCK_SIZE]
    // with no knowledge of the actual block size. This function will set it.
    if((ivInOut != NULL) && (mode != TPM_ALG_ECB))
    {
        ivInOut->t.size = blockSize;
        iv              = ivInOut->t.buffer;
    }
    else
        iv = defaultIv;
    pIv = iv;

    // Create encrypt key schedule and set the encryption function pointer.
    switch(algorithm)
    {
        FOR_EACH_SYM(ENCRYPT_CASE)

        default:
            return TPM_RC_SYMMETRIC;
    }
    switch(mode)
    {
#if ALG_CTR
        case TPM_ALG_CTR:
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the current value of the IV(counter)
                ENCRYPT(&keySchedule, iv, tmp);

                //increment the counter (counter is big-endian so start at end)
                for(i = blockSize - 1; i >= 0; i--)
                    if((iv[i] += 1) != 0)
                        break;
                // XOR the encrypted counter value with input and put into output
                pT = tmp;
                for(i = (dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    *dOut++ = *dIn++ ^ *pT++;
            }
            break;
#endif
#if ALG_OFB
        case TPM_ALG_OFB:
            // This is written so that dIn and dOut may be the same
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the current value of the "IV"
                ENCRYPT(&keySchedule, iv, iv);

                // XOR the encrypted IV into dIn to create the cipher text (dOut)
                pIv = iv;
                for(i = (dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    *dOut++ = (*pIv++ ^ *dIn++);
            }
            break;
#endif
#if ALG_CBC
        case TPM_ALG_CBC:
            // For CBC the data size must be an even multiple of the
            // cipher block size
            if((dSize % blockSize) != 0)
                return TPM_RC_SIZE;
            // XOR the data block into the IV, encrypt the IV into the IV
            // and then copy the IV to the output
            for(; dSize > 0; dSize -= blockSize)
            {
                pIv = iv;
                for(i = blockSize; i > 0; i--)
                    *pIv++ ^= *dIn++;
                ENCRYPT(&keySchedule, iv, iv);
                pIv = iv;
                for(i = blockSize; i > 0; i--)
                    *dOut++ = *pIv++;
            }
            break;
#endif
        // CFB is not optional
        case TPM_ALG_CFB:
            // Encrypt the IV into the IV, XOR in the data, and copy to output
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the current value of the IV
                ENCRYPT(&keySchedule, iv, iv);
                pIv = iv;
                for(i = (int)(dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    // XOR the data into the IV to create the cipher text
                    // and put into the output
                    *dOut++ = *pIv++ ^= *dIn++;
            }
            // If the inner loop (i loop) was smaller than blockSize, then dSize
            // would have been smaller than blockSize and it is now negative. If
            // it is negative, then it indicates how many bytes are needed to pad
            // out the IV for the next round.
            for(; dSize < 0; dSize++)
                *pIv++ = 0;
            break;
#if ALG_ECB
        case TPM_ALG_ECB:
            // For ECB the data size must be an even multiple of the
            // cipher block size
            if((dSize % blockSize) != 0)
                return TPM_RC_SIZE;
            // Encrypt the input block to the output block
            for(; dSize > 0; dSize -= blockSize)
            {
                ENCRYPT(&keySchedule, dIn, dOut);
                dIn  = &dIn[blockSize];
                dOut = &dOut[blockSize];
            }
            break;
#endif
        default:
            return TPM_RC_FAILURE;
    }
    return TPM_RC_SUCCESS;
}

//*** CryptSymmetricDecrypt()
// This function performs symmetric decryption based on the mode.
//  Return Type: TPM_RC
//      TPM_RC_FAILURE      A fatal error
//      TPM_RCS_SIZE        'dSize' is not a multiple of the block size for an
//                          algorithm that requires it
LIB_EXPORT TPM_RC CryptSymmetricDecrypt(
    BYTE*       dOut,           // OUT: decrypted data
    TPM_ALG_ID  algorithm,      // IN: the symmetric algorithm
    UINT16      keySizeInBits,  // IN: key size in bits
    const BYTE* key,            // IN: key buffer. The size of this buffer
                                //     in bytes is (keySizeInBits + 7) / 8
    TPM2B_IV*  ivInOut,         // IN/OUT: IV for decryption.
    TPM_ALG_ID mode,            // IN: Mode to use
    INT32      dSize,           // IN: data size (may need to be a
                                //     multiple of the blockSize)
    const BYTE* dIn             // IN: data buffer
)
{
    BYTE*                   pIv;
    int                     i;
    BYTE                    tmp[MAX_SYM_BLOCK_SIZE];
    BYTE*                   pT;
    tpmCryptKeySchedule_t   keySchedule;
    INT16                   blockSize;
    BYTE*                   iv;
    TpmCryptSetSymKeyCall_t encrypt;
    TpmCryptSetSymKeyCall_t decrypt;
    BYTE                    defaultIv[MAX_SYM_BLOCK_SIZE] = {0};

    // These are used but the compiler can't tell because they are initialized
    // in case statements and it can't tell if they are always initialized
    // when needed, so... Comment these out if the compiler can tell or doesn't
    // care that these are initialized before use.
    encrypt = NULL;
    decrypt = NULL;

    pAssert(dOut != NULL && key != NULL && dIn != NULL);
    if(dSize == 0)
        return TPM_RC_SUCCESS;

    TPM_DO_SELF_TEST(algorithm);
    blockSize = CryptGetSymmetricBlockSize(algorithm, keySizeInBits);
    if(blockSize == 0)
        return TPM_RC_FAILURE;
    // If the iv is provided, then it is expected to be block sized. In some cases,
    // the caller is providing an array of 0's that is equal to [MAX_SYM_BLOCK_SIZE]
    // with no knowledge of the actual block size. This function will set it.
    if((ivInOut != NULL) && (mode != TPM_ALG_ECB))
    {
        ivInOut->t.size = blockSize;
        iv              = ivInOut->t.buffer;
    }
    else
        iv = defaultIv;

    pIv = iv;
    // Use the mode to select the key schedule to create. Encrypt always uses the
    // encryption schedule. Depending on the mode, decryption might use either
    // the decryption or encryption schedule.
    switch(mode)
    {
#if ALG_CBC || ALG_ECB
        case TPM_ALG_CBC:  // decrypt = decrypt
        case TPM_ALG_ECB:
            // For ECB and CBC, the data size must be an even multiple of the
            // cipher block size
            if((dSize % blockSize) != 0)
                return TPM_RC_SIZE;
            switch(algorithm)
            {
                FOR_EACH_SYM(DECRYPT_CASE)
                default:
                    return TPM_RC_SYMMETRIC;
            }
            break;
#endif
        default:
            // For the remaining stream ciphers, use encryption to decrypt
            switch(algorithm)
            {
                FOR_EACH_SYM(ENCRYPT_CASE)
                default:
                    return TPM_RC_SYMMETRIC;
            }
    }
    // Now do the mode-dependent decryption
    switch(mode)
    {
#if ALG_CBC
        case TPM_ALG_CBC:
            // Copy the input data to a temp buffer, decrypt the buffer into the
            // output, XOR in the IV, and copy the temp buffer to the IV and repeat.
            for(; dSize > 0; dSize -= blockSize)
            {
                pT = tmp;
                for(i = blockSize; i > 0; i--)
                    *pT++ = *dIn++;
                DECRYPT(&keySchedule, tmp, dOut);
                pIv = iv;
                pT  = tmp;
                for(i = blockSize; i > 0; i--)
                {
                    *dOut++ ^= *pIv;
                    *pIv++ = *pT++;
                }
            }
            break;
#endif
        case TPM_ALG_CFB:
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the IV into the temp buffer
                ENCRYPT(&keySchedule, iv, tmp);
                pT  = tmp;
                pIv = iv;
                for(i = (dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    // Copy the current cipher text to IV, XOR
                    // with the temp buffer and put into the output
                    *dOut++ = *pT++ ^ (*pIv++ = *dIn++);
            }
            // If the inner loop (i loop) was smaller than blockSize, then dSize
            // would have been smaller than blockSize and it is now negative
            // If it is negative, then it indicates how may fill bytes
            // are needed to pad out the IV for the next round.
            for(; dSize < 0; dSize++)
                *pIv++ = 0;

            break;
#if ALG_CTR
        case TPM_ALG_CTR:
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the current value of the IV(counter)
                ENCRYPT(&keySchedule, iv, tmp);

                //increment the counter (counter is big-endian so start at end)
                for(i = blockSize - 1; i >= 0; i--)
                    if((iv[i] += 1) != 0)
                        break;
                // XOR the encrypted counter value with input and put into output
                pT = tmp;
                for(i = (dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    *dOut++ = *dIn++ ^ *pT++;
            }
            break;
#endif
#if ALG_ECB
        case TPM_ALG_ECB:
            for(; dSize > 0; dSize -= blockSize)
            {
                DECRYPT(&keySchedule, dIn, dOut);
                dIn  = &dIn[blockSize];
                dOut = &dOut[blockSize];
            }
            break;
#endif
#if ALG_OFB
        case TPM_ALG_OFB:
            // This is written so that dIn and dOut may be the same
            for(; dSize > 0; dSize -= blockSize)
            {
                // Encrypt the current value of the "IV"
                ENCRYPT(&keySchedule, iv, iv);

                // XOR the encrypted IV into dIn to create the cipher text (dOut)
                pIv = iv;
                for(i = (dSize < blockSize) ? dSize : blockSize; i > 0; i--)
                    *dOut++ = (*pIv++ ^ *dIn++);
            }
            break;
#endif
        default:
            return TPM_RC_FAILURE;
    }
    return TPM_RC_SUCCESS;
}

//*** CryptSymKeyValidate()
// Validate that a provided symmetric key meets the requirements of the TPM
//  Return Type: TPM_RC
//      TPM_RC_KEY_SIZE         Key size specifiers do not match
//      TPM_RC_KEY              Key is not allowed
TPM_RC
CryptSymKeyValidate(TPMT_SYM_DEF_OBJECT* symDef, TPM2B_SYM_KEY* key)
{
    if(key->t.size != BITS_TO_BYTES(symDef->keyBits.sym))
        return TPM_RCS_KEY_SIZE;
    return TPM_RC_SUCCESS;
}
