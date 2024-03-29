#include "Tpm.h"
#include "EncryptDecrypt_fp.h"
#include "EncryptDecrypt_spt_fp.h"

#if CC_EncryptDecrypt2

/*(See part 3 specification)
// symmetric encryption or decryption
*/
//  Return Type: TPM_RC
//      TPM_RC_KEY          is not a symmetric decryption key with both
//                          public and private portions loaded
//      TPM_RC_SIZE         'IvIn' size is incompatible with the block cipher mode;
//                          or 'inData' size is not an even multiple of the block
//                          size for CBC or ECB mode
//      TPM_RC_VALUE        'keyHandle' is restricted and the argument 'mode' does
//                          not match the key's mode
TPM_RC
EncryptDecryptShared(TPMI_DH_OBJECT      keyHandleIn,
                     TPMI_YES_NO         decryptIn,
                     TPMI_ALG_SYM_MODE   modeIn,
                     TPM2B_IV*           ivIn,
                     TPM2B_MAX_BUFFER*   inData,
                     EncryptDecrypt_Out* out)
{
    OBJECT*    symKey;
    UINT16     keySize;
    UINT16     blockSize;
    BYTE*      key;
    TPM_ALG_ID alg;
    TPM_ALG_ID mode;
    TPM_RC     result;
    BOOL       OK;
    // Input Validation
    symKey = HandleToObject(keyHandleIn);
    mode   = symKey->publicArea.parameters.symDetail.sym.mode.sym;

    // The input key should be a symmetric key
    if(symKey->publicArea.type != TPM_ALG_SYMCIPHER)
        return TPM_RCS_KEY + RC_EncryptDecrypt_keyHandle;
    // The key must be unrestricted and allow the selected operation
    OK = !IS_ATTRIBUTE(symKey->publicArea.objectAttributes, TPMA_OBJECT, restricted);
    if(YES == decryptIn)
        OK = OK
             && IS_ATTRIBUTE(
                 symKey->publicArea.objectAttributes, TPMA_OBJECT, decrypt);
    else
        OK = OK
             && IS_ATTRIBUTE(symKey->publicArea.objectAttributes, TPMA_OBJECT, sign);
    if(!OK)
        return TPM_RCS_ATTRIBUTES + RC_EncryptDecrypt_keyHandle;

    // Make sure that key is an encrypt/decrypt key and not SMAC
    if(!CryptSymModeIsValid(mode, TRUE))
        return TPM_RCS_MODE + RC_EncryptDecrypt_keyHandle;

    // If the key mode is not TPM_ALG_NULL...
    // or TPM_ALG_NULL
    if(mode != TPM_ALG_NULL)
    {
        // then the input mode has to be TPM_ALG_NULL or the same as the key
        if((modeIn != TPM_ALG_NULL) && (modeIn != mode))
            return TPM_RCS_MODE + RC_EncryptDecrypt_mode;
    }
    else
    {
        // if the key mode is null, then the input can't be null
        if(modeIn == TPM_ALG_NULL)
            return TPM_RCS_MODE + RC_EncryptDecrypt_mode;
        mode = modeIn;
    }
    // The input iv for ECB mode should be an Empty Buffer.  All the other modes
    // should have an iv size same as encryption block size
    keySize   = symKey->publicArea.parameters.symDetail.sym.keyBits.sym;
    alg       = symKey->publicArea.parameters.symDetail.sym.algorithm;
    blockSize = CryptGetSymmetricBlockSize(alg, keySize);

    // reverify the algorithm. This is mainly to keep static analysis tools happy
    if(blockSize == 0)
        return TPM_RCS_KEY + RC_EncryptDecrypt_keyHandle;

    if(((mode == TPM_ALG_ECB) && (ivIn->t.size != 0))
       || ((mode != TPM_ALG_ECB) && (ivIn->t.size != blockSize)))
        return TPM_RCS_SIZE + RC_EncryptDecrypt_ivIn;

    // The input data size of CBC mode or ECB mode must be an even multiple of
    // the symmetric algorithm's block size
    if(((mode == TPM_ALG_CBC) || (mode == TPM_ALG_ECB))
       && ((inData->t.size % blockSize) != 0))
        return TPM_RCS_SIZE + RC_EncryptDecrypt_inData;

    // Copy IV
    // Note: This is copied here so that the calls to the encrypt/decrypt functions
    // will modify the output buffer, not the input buffer
    out->ivOut = *ivIn;

    // Command Output
    key = symKey->sensitive.sensitive.sym.t.buffer;
    // For symmetric encryption, the cipher data size is the same as plain data
    // size.
    out->outData.t.size = inData->t.size;
    if(decryptIn == YES)
    {
        // Decrypt data to output
        result = CryptSymmetricDecrypt(out->outData.t.buffer,
                                       alg,
                                       keySize,
                                       key,
                                       &(out->ivOut),
                                       mode,
                                       inData->t.size,
                                       inData->t.buffer);
    }
    else
    {
        // Encrypt data to output
        result = CryptSymmetricEncrypt(out->outData.t.buffer,
                                       alg,
                                       keySize,
                                       key,
                                       &(out->ivOut),
                                       mode,
                                       inData->t.size,
                                       inData->t.buffer);
    }
    return result;
}

#endif  // CC_EncryptDecrypt