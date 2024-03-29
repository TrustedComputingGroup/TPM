
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PolicyLocality  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYLOCALITY_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYLOCALITY_FP_H_

// Input structure definition
typedef struct
{
    TPMI_SH_POLICY policySession;
    TPMA_LOCALITY  locality;
} PolicyLocality_In;

// Response code modifiers
#    define RC_PolicyLocality_policySession (TPM_RC_H + TPM_RC_1)
#    define RC_PolicyLocality_locality      (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyLocality(PolicyLocality_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYLOCALITY_FP_H_
#endif    // CC_PolicyLocality
