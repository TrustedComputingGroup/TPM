
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_FirmwareRead  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIRMWAREREAD_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIRMWAREREAD_FP_H_

// Input structure definition
typedef struct
{
    UINT32 sequenceNumber;
} FirmwareRead_In;

// Output structure definition
typedef struct
{
    TPM2B_MAX_BUFFER fuData;
} FirmwareRead_Out;

// Response code modifiers
#    define RC_FirmwareRead_sequenceNumber (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_FirmwareRead(FirmwareRead_In* in, FirmwareRead_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIRMWAREREAD_FP_H_
#endif    // CC_FirmwareRead