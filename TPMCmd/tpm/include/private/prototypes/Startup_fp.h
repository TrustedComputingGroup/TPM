
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Startup  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_STARTUP_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_STARTUP_FP_H_

// Input structure definition
typedef struct
{
    TPM_SU startupType;
} Startup_In;

// Response code modifiers
#    define RC_Startup_startupType (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Startup(Startup_In* in);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_STARTUP_FP_H_
#endif    // CC_Startup