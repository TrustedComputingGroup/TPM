
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Rewrap  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_REWRAP_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_REWRAP_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT         oldParent;
    TPMI_DH_OBJECT         newParent;
    TPM2B_PRIVATE          inDuplicate;
    TPM2B_NAME             name;
    TPM2B_ENCRYPTED_SECRET inSymSeed;
} Rewrap_In;

// Output structure definition
typedef struct
{
    TPM2B_PRIVATE          outDuplicate;
    TPM2B_ENCRYPTED_SECRET outSymSeed;
} Rewrap_Out;

// Response code modifiers
#    define RC_Rewrap_oldParent   (TPM_RC_H + TPM_RC_1)
#    define RC_Rewrap_newParent   (TPM_RC_H + TPM_RC_2)
#    define RC_Rewrap_inDuplicate (TPM_RC_P + TPM_RC_1)
#    define RC_Rewrap_name        (TPM_RC_P + TPM_RC_2)
#    define RC_Rewrap_inSymSeed   (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Rewrap(Rewrap_In* in, Rewrap_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_REWRAP_FP_H_
#endif    // CC_Rewrap
