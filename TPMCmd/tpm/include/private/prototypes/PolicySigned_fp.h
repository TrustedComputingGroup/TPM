
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_PolicySigned  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYSIGNED_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYSIGNED_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT authObject;
    TPMI_SH_POLICY policySession;
    TPM2B_NONCE    nonceTPM;
    TPM2B_DIGEST   cpHashA;
    TPM2B_NONCE    policyRef;
    INT32          expiration;
    TPMT_SIGNATURE auth;
} PolicySigned_In;

// Output structure definition
typedef struct
{
    TPM2B_TIMEOUT timeout;
    TPMT_TK_AUTH  policyTicket;
} PolicySigned_Out;

// Response code modifiers
#    define RC_PolicySigned_authObject    (TPM_RC_H + TPM_RC_1)
#    define RC_PolicySigned_policySession (TPM_RC_H + TPM_RC_2)
#    define RC_PolicySigned_nonceTPM      (TPM_RC_P + TPM_RC_1)
#    define RC_PolicySigned_cpHashA       (TPM_RC_P + TPM_RC_2)
#    define RC_PolicySigned_policyRef     (TPM_RC_P + TPM_RC_3)
#    define RC_PolicySigned_expiration    (TPM_RC_P + TPM_RC_4)
#    define RC_PolicySigned_auth          (TPM_RC_P + TPM_RC_5)

// Function prototype
TPM_RC
TPM2_PolicySigned(PolicySigned_In* in, PolicySigned_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_POLICYSIGNED_FP_H_
#endif    // CC_PolicySigned