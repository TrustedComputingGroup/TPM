
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_NV_SetBits  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_SETBITS_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_SETBITS_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_NV_AUTH  authHandle;
    TPMI_RH_NV_INDEX nvIndex;
    UINT64           bits;
} NV_SetBits_In;

// Response code modifiers
#    define RC_NV_SetBits_authHandle (TPM_RC_H + TPM_RC_1)
#    define RC_NV_SetBits_nvIndex    (TPM_RC_H + TPM_RC_2)
#    define RC_NV_SetBits_bits       (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_SetBits(NV_SetBits_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_NV_SETBITS_FP_H_
#endif    // CC_NV_SetBits