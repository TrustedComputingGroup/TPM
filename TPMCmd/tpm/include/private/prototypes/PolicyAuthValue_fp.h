
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PolicyAuthValue  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYAUTHVALUE_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYAUTHVALUE_FP_H_

// Input structure definition
typedef struct
{
    TPMI_SH_POLICY policySession;
} PolicyAuthValue_In;

// Response code modifiers
#    define RC_PolicyAuthValue_policySession (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyAuthValue(PolicyAuthValue_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYAUTHVALUE_FP_H_
#endif    // CC_PolicyAuthValue
