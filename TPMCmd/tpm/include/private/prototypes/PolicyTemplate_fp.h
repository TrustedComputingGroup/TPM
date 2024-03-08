
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PolicyTemplate  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYTEMPLATE_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYTEMPLATE_FP_H_

// Input structure definition
typedef struct
{
    TPMI_SH_POLICY policySession;
    TPM2B_DIGEST   templateHash;
} PolicyTemplate_In;

// Response code modifiers
#    define RC_PolicyTemplate_policySession (TPM_RC_H + TPM_RC_1)
#    define RC_PolicyTemplate_templateHash  (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyTemplate(PolicyTemplate_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYTEMPLATE_FP_H_
#endif    // CC_PolicyTemplate