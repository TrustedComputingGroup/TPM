
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PolicyDuplicationSelect  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYDUPLICATIONSELECT_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYDUPLICATIONSELECT_FP_H_

// Input structure definition
typedef struct
{
    TPMI_SH_POLICY policySession;
    TPM2B_NAME     objectName;
    TPM2B_NAME     newParentName;
    TPMI_YES_NO    includeObject;
} PolicyDuplicationSelect_In;

// Response code modifiers
#    define RC_PolicyDuplicationSelect_policySession (TPM_RC_H + TPM_RC_1)
#    define RC_PolicyDuplicationSelect_objectName    (TPM_RC_P + TPM_RC_1)
#    define RC_PolicyDuplicationSelect_newParentName (TPM_RC_P + TPM_RC_2)
#    define RC_PolicyDuplicationSelect_includeObject (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PolicyDuplicationSelect(PolicyDuplicationSelect_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYDUPLICATIONSELECT_FP_H_
#endif    // CC_PolicyDuplicationSelect
