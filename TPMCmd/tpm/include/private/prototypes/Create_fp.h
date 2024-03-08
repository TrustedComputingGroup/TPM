
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Create  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_CREATE_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_CREATE_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT         parentHandle;
    TPM2B_SENSITIVE_CREATE inSensitive;
    TPM2B_PUBLIC           inPublic;
    TPM2B_DATA             outsideInfo;
    TPML_PCR_SELECTION     creationPCR;
} Create_In;

// Output structure definition
typedef struct
{
    TPM2B_PRIVATE       outPrivate;
    TPM2B_PUBLIC        outPublic;
    TPM2B_CREATION_DATA creationData;
    TPM2B_DIGEST        creationHash;
    TPMT_TK_CREATION    creationTicket;
} Create_Out;

// Response code modifiers
#    define RC_Create_parentHandle (TPM_RC_H + TPM_RC_1)
#    define RC_Create_inSensitive  (TPM_RC_P + TPM_RC_1)
#    define RC_Create_inPublic     (TPM_RC_P + TPM_RC_2)
#    define RC_Create_outsideInfo  (TPM_RC_P + TPM_RC_3)
#    define RC_Create_creationPCR  (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_Create(Create_In* in, Create_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_CREATE_FP_H_
#endif    // CC_Create