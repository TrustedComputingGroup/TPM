
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_CertifyCreation  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_CERTIFYCREATION_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_CERTIFYCREATION_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT   signHandle;
    TPMI_DH_OBJECT   objectHandle;
    TPM2B_DATA       qualifyingData;
    TPM2B_DIGEST     creationHash;
    TPMT_SIG_SCHEME  inScheme;
    TPMT_TK_CREATION creationTicket;
} CertifyCreation_In;

// Output structure definition
typedef struct
{
    TPM2B_ATTEST   certifyInfo;
    TPMT_SIGNATURE signature;
} CertifyCreation_Out;

// Response code modifiers
#    define RC_CertifyCreation_signHandle     (TPM_RC_H + TPM_RC_1)
#    define RC_CertifyCreation_objectHandle   (TPM_RC_H + TPM_RC_2)
#    define RC_CertifyCreation_qualifyingData (TPM_RC_P + TPM_RC_1)
#    define RC_CertifyCreation_creationHash   (TPM_RC_P + TPM_RC_2)
#    define RC_CertifyCreation_inScheme       (TPM_RC_P + TPM_RC_3)
#    define RC_CertifyCreation_creationTicket (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_CertifyCreation(CertifyCreation_In* in, CertifyCreation_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_CERTIFYCREATION_FP_H_
#endif    // CC_CertifyCreation