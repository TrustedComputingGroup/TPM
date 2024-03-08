
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_Hash  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_HASH_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_HASH_FP_H_

// Input structure definition
typedef struct
{
    TPM2B_MAX_BUFFER  data;
    TPMI_ALG_HASH     hashAlg;
    TPMI_RH_HIERARCHY hierarchy;
} Hash_In;

// Output structure definition
typedef struct
{
    TPM2B_DIGEST      outHash;
    TPMT_TK_HASHCHECK validation;
} Hash_Out;

// Response code modifiers
#    define RC_Hash_data      (TPM_RC_P + TPM_RC_1)
#    define RC_Hash_hashAlg   (TPM_RC_P + TPM_RC_2)
#    define RC_Hash_hierarchy (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Hash(Hash_In* in, Hash_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_HASH_FP_H_
#endif    // CC_Hash