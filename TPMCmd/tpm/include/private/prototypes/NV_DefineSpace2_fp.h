
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_NV_DefineSpace2  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_DEFINESPACE2_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_DEFINESPACE2_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_PROVISION authHandle;
    TPM2B_AUTH        auth;
    TPM2B_NV_PUBLIC_2 publicInfo;
} NV_DefineSpace2_In;

// Response code modifiers
#    define RC_NV_DefineSpace2_authHandle (TPM_RC_H + TPM_RC_1)
#    define RC_NV_DefineSpace2_auth       (TPM_RC_P + TPM_RC_1)
#    define RC_NV_DefineSpace2_publicInfo (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_NV_DefineSpace2(NV_DefineSpace2_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_DEFINESPACE2_FP_H_
#endif    // CC_NV_DefineSpace2