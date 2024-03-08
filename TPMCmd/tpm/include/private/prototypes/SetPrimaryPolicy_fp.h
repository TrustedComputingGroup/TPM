
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_SetPrimaryPolicy  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_SETPRIMARYPOLICY_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_SETPRIMARYPOLICY_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_HIERARCHY_POLICY authHandle;
    TPM2B_DIGEST             authPolicy;
    TPMI_ALG_HASH            hashAlg;
} SetPrimaryPolicy_In;

// Response code modifiers
#    define RC_SetPrimaryPolicy_authHandle (TPM_RC_H + TPM_RC_1)
#    define RC_SetPrimaryPolicy_authPolicy (TPM_RC_P + TPM_RC_1)
#    define RC_SetPrimaryPolicy_hashAlg    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_SetPrimaryPolicy(SetPrimaryPolicy_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_SETPRIMARYPOLICY_FP_H_
#endif    // CC_SetPrimaryPolicy