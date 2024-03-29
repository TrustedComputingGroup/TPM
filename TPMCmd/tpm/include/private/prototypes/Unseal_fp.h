
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Unseal  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_UNSEAL_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_UNSEAL_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT itemHandle;
} Unseal_In;

// Output structure definition
typedef struct
{
    TPM2B_SENSITIVE_DATA outData;
} Unseal_Out;

// Response code modifiers
#    define RC_Unseal_itemHandle (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Unseal(Unseal_In* in, Unseal_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_UNSEAL_FP_H_
#endif    // CC_Unseal
