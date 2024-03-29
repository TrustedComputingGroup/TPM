
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_GetRandom  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETRANDOM_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETRANDOM_FP_H_

// Input structure definition
typedef struct
{
    UINT16 bytesRequested;
} GetRandom_In;

// Output structure definition
typedef struct
{
    TPM2B_DIGEST randomBytes;
} GetRandom_Out;

// Response code modifiers
#    define RC_GetRandom_bytesRequested (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_GetRandom(GetRandom_In* in, GetRandom_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_GETRANDOM_FP_H_
#endif    // CC_GetRandom
