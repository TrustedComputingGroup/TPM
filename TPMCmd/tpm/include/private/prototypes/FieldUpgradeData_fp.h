
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_FieldUpgradeData  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIELDUPGRADEDATA_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIELDUPGRADEDATA_FP_H_

// Input structure definition
typedef struct
{
    TPM2B_MAX_BUFFER fuData;
} FieldUpgradeData_In;

// Output structure definition
typedef struct
{
    TPMT_HA nextDigest;
    TPMT_HA firstDigest;
} FieldUpgradeData_Out;

// Response code modifiers
#    define RC_FieldUpgradeData_fuData (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_FieldUpgradeData(FieldUpgradeData_In* in, FieldUpgradeData_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_FIELDUPGRADEDATA_FP_H_
#endif    // CC_FieldUpgradeData