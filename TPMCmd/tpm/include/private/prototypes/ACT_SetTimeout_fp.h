
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_ACT_SetTimeout  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_ACT_SETTIMEOUT_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_ACT_SETTIMEOUT_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_ACT actHandle;
    UINT32      startTimeout;
} ACT_SetTimeout_In;

// Response code modifiers
#    define RC_ACT_SetTimeout_actHandle    (TPM_RC_H + TPM_RC_1)
#    define RC_ACT_SetTimeout_startTimeout (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ACT_SetTimeout(ACT_SetTimeout_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_ACT_SETTIMEOUT_FP_H_
#endif    // CC_ACT_SetTimeout