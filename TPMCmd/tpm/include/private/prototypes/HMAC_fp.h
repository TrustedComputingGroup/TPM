
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_HMAC  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_HMAC_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_HMAC_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT   handle;
    TPM2B_MAX_BUFFER buffer;
    TPMI_ALG_HASH    hashAlg;
} HMAC_In;

// Output structure definition
typedef struct
{
    TPM2B_DIGEST outHMAC;
} HMAC_Out;

// Response code modifiers
#    define RC_HMAC_handle  (TPM_RC_H + TPM_RC_1)
#    define RC_HMAC_buffer  (TPM_RC_P + TPM_RC_1)
#    define RC_HMAC_hashAlg (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HMAC(HMAC_In* in, HMAC_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_HMAC_FP_H_
#endif    // CC_HMAC