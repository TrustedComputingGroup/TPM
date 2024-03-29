
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_GetCapability  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETCAPABILITY_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETCAPABILITY_FP_H_

// Input structure definition
typedef struct
{
    TPM_CAP capability;
    UINT32  property;
    UINT32  propertyCount;
} GetCapability_In;

// Output structure definition
typedef struct
{
    TPMI_YES_NO          moreData;
    TPMS_CAPABILITY_DATA capabilityData;
} GetCapability_Out;

// Response code modifiers
#    define RC_GetCapability_capability    (TPM_RC_P + TPM_RC_1)
#    define RC_GetCapability_property      (TPM_RC_P + TPM_RC_2)
#    define RC_GetCapability_propertyCount (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_GetCapability(GetCapability_In* in, GetCapability_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETCAPABILITY_FP_H_
#endif    // CC_GetCapability
