
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PCR_SetAuthValue  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_PCR_SETAUTHVALUE_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_PCR_SETAUTHVALUE_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_PCR  pcrHandle;
    TPM2B_DIGEST auth;
} PCR_SetAuthValue_In;

// Response code modifiers
#    define RC_PCR_SetAuthValue_pcrHandle (TPM_RC_H + TPM_RC_1)
#    define RC_PCR_SetAuthValue_auth      (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_SetAuthValue(PCR_SetAuthValue_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_PCR_SETAUTHVALUE_FP_H_
#endif    // CC_PCR_SetAuthValue
