
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_ChangeEPS  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_CHANGEEPS_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_CHANGEEPS_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_PLATFORM authHandle;
} ChangeEPS_In;

// Response code modifiers
#    define RC_ChangeEPS_authHandle (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ChangeEPS(ChangeEPS_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_CHANGEEPS_FP_H_
#endif    // CC_ChangeEPS