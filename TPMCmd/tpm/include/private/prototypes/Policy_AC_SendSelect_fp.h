
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Policy_AC_SendSelect  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICY_AC_SENDSELECT_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICY_AC_SENDSELECT_FP_H_

// Input structure definition
typedef struct
{
    TPMI_SH_POLICY policySession;
    TPM2B_NAME     objectName;
    TPM2B_NAME     authHandleName;
    TPM2B_NAME     acName;
    TPMI_YES_NO    includeObject;
} Policy_AC_SendSelect_In;

// Response code modifiers
#    define RC_Policy_AC_SendSelect_policySession  (TPM_RC_H + TPM_RC_1)
#    define RC_Policy_AC_SendSelect_objectName     (TPM_RC_P + TPM_RC_1)
#    define RC_Policy_AC_SendSelect_authHandleName (TPM_RC_P + TPM_RC_2)
#    define RC_Policy_AC_SendSelect_acName         (TPM_RC_P + TPM_RC_3)
#    define RC_Policy_AC_SendSelect_includeObject  (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_Policy_AC_SendSelect(Policy_AC_SendSelect_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICY_AC_SENDSELECT_FP_H_
#endif    // CC_Policy_AC_SendSelect