
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_ContextLoad  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_CONTEXTLOAD_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_CONTEXTLOAD_FP_H_

// Input structure definition
typedef struct
{
    TPMS_CONTEXT context;
} ContextLoad_In;

// Output structure definition
typedef struct
{
    TPMI_DH_CONTEXT loadedHandle;
} ContextLoad_Out;

// Response code modifiers
#    define RC_ContextLoad_context (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ContextLoad(ContextLoad_In* in, ContextLoad_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_CONTEXTLOAD_FP_H_
#endif    // CC_ContextLoad
