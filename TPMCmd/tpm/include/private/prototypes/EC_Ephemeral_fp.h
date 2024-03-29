
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_EC_Ephemeral  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_EC_EPHEMERAL_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_EC_EPHEMERAL_FP_H_

// Input structure definition
typedef struct
{
    TPMI_ECC_CURVE curveID;
} EC_Ephemeral_In;

// Output structure definition
typedef struct
{
    TPM2B_ECC_POINT Q;
    UINT16          counter;
} EC_Ephemeral_Out;

// Response code modifiers
#    define RC_EC_Ephemeral_curveID (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_EC_Ephemeral(EC_Ephemeral_In* in, EC_Ephemeral_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_EC_EPHEMERAL_FP_H_
#endif    // CC_EC_Ephemeral
