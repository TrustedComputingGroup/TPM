/*(Auto-generated)
 *  Created by TpmPrototypes; Version 3.0 July 18, 2017
 *  Date: Apr  2, 2019  Time: 03:18:00PM
 */

#ifndef _CRYPT_SYM_FP_H_
#define _CRYPT_SYM_FP_H_

//** Initialization and Data Access Functions
//
//*** CryptSymInit()
// This function is called to do _TPM_Init processing
BOOL CryptSymInit(void);

//*** CryptSymStartup()
// This function is called to do TPM2_Startup() processing
BOOL CryptSymStartup(void);

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
);

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
);

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
);

//*** CryptSymKeyValidate()
// Validate that a provided symmetric key meets the requirements of the TPM
//  Return Type: TPM_RC
//      TPM_RC_KEY_SIZE         Key size specifiers do not match
//      TPM_RC_KEY              Key is not allowed
TPM_RC
CryptSymKeyValidate(TPMT_SYM_DEF_OBJECT* symDef, TPM2B_SYM_KEY* key);

#endif  // _CRYPT_SYM_FP_H_
