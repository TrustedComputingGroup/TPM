
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_EventSequenceComplete  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_EVENTSEQUENCECOMPLETE_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_EVENTSEQUENCECOMPLETE_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_PCR      pcrHandle;
    TPMI_DH_OBJECT   sequenceHandle;
    TPM2B_MAX_BUFFER buffer;
} EventSequenceComplete_In;

// Output structure definition
typedef struct
{
    TPML_DIGEST_VALUES results;
} EventSequenceComplete_Out;

// Response code modifiers
#    define RC_EventSequenceComplete_pcrHandle      (TPM_RC_H + TPM_RC_1)
#    define RC_EventSequenceComplete_sequenceHandle (TPM_RC_H + TPM_RC_2)
#    define RC_EventSequenceComplete_buffer         (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_EventSequenceComplete(EventSequenceComplete_In*  in,
                           EventSequenceComplete_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_EVENTSEQUENCECOMPLETE_FP_H_
#endif    // CC_EventSequenceComplete