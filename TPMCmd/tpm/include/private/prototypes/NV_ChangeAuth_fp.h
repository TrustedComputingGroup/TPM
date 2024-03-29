
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_NV_ChangeAuth  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_CHANGEAUTH_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_CHANGEAUTH_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_NV_INDEX nvIndex;
    TPM2B_AUTH       newAuth;
} NV_ChangeAuth_In;

// Response code modifiers
#    define RC_NV_ChangeAuth_nvIndex (TPM_RC_H + TPM_RC_1)
#    define RC_NV_ChangeAuth_newAuth (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_ChangeAuth(NV_ChangeAuth_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_CHANGEAUTH_FP_H_
#endif    // CC_NV_ChangeAuth
