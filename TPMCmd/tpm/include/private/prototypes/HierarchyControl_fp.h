
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_HierarchyControl  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCONTROL_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCONTROL_FP_H_

// Input structure definition
typedef struct
{
    TPMI_RH_BASE_HIERARCHY authHandle;
    TPMI_RH_ENABLES        enable;
    TPMI_YES_NO            state;
} HierarchyControl_In;

// Response code modifiers
#    define RC_HierarchyControl_authHandle (TPM_RC_H + TPM_RC_1)
#    define RC_HierarchyControl_enable     (TPM_RC_P + TPM_RC_1)
#    define RC_HierarchyControl_state      (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HierarchyControl(HierarchyControl_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_HIERARCHYCONTROL_FP_H_
#endif    // CC_HierarchyControl