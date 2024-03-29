
// FILE GENERATED BY TpmExtractCode: DO NOT EDIT

#if CC_AC_Send  // Command must be enabled

#  ifndef _TPM_INCLUDE_PRIVATE_PROTOTYPES_AC_SEND_FP_H_
#    define _TPM_INCLUDE_PRIVATE_PROTOTYPES_AC_SEND_FP_H_

// Input structure definition
typedef struct
{
    TPMI_DH_OBJECT   sendObject;
    TPMI_RH_NV_AUTH  authHandle;
    TPMI_RH_AC       ac;
    TPM2B_MAX_BUFFER acDataIn;
} AC_Send_In;

// Output structure definition
typedef struct
{
    TPMS_AC_OUTPUT acDataOut;
} AC_Send_Out;

// Response code modifiers
#    define RC_AC_Send_sendObject (TPM_RC_H + TPM_RC_1)
#    define RC_AC_Send_authHandle (TPM_RC_H + TPM_RC_2)
#    define RC_AC_Send_ac         (TPM_RC_H + TPM_RC_3)
#    define RC_AC_Send_acDataIn   (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_AC_Send(AC_Send_In* in, AC_Send_Out* out);

#  endif  // _TPM_INCLUDE_PRIVATE_PROTOTYPES_AC_SEND_FP_H_
#endif    // CC_AC_Send
