
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_HierarchyChangeAuth  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCHANGEAUTH_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCHANGEAUTH_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_HIERARCHY_AUTH authHandle;
    TPM2B_AUTH             newAuth;
} HierarchyChangeAuth_In;

// Response code modifiers
#    define RC_HierarchyChangeAuth_authHandle (TPM_RC_H + TPM_RC_1)
#    define RC_HierarchyChangeAuth_newAuth    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_HierarchyChangeAuth(HierarchyChangeAuth_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCHANGEAUTH_FP_H_
#endif    // CC_HierarchyChangeAuth