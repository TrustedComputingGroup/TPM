/*(Auto-generated)
 *  Created by TpmPrototypes; Version 3.0 July 18, 2017
 *  Date: Apr  2, 2019  Time: 04:23:27PM
 */

#ifndef _COMMAND_AUDIT_FP_H_
#define _COMMAND_AUDIT_FP_H_

//*** CommandAuditPreInstall_Init()
// This function initializes the command audit list. This function simulates
// the behavior of manufacturing. A function is used instead of a structure
// definition because this is easier than figuring out the initialization value
// for a bit array.
//
// This function would not be implemented outside of a manufacturing or
// simulation environment.
void CommandAuditPreInstall_Init(void);

//*** CommandAuditStartup()
// This function clears the command audit digest on a TPM Reset.
BOOL CommandAuditStartup(STARTUP_TYPE type  // IN: start up type
);

//*** CommandAuditSet()
// This function will SET the audit flag for a command. This function
// will not SET the audit flag for a command that is not implemented. This
// ensures that the audit status is not SET when TPM2_GetCapability() is
// used to read the list of audited commands.
//
// This function is only used by TPM2_SetCommandCodeAuditStatus().
//
// The actions in TPM2_SetCommandCodeAuditStatus() are expected to cause the
// changes to be saved to NV after it is setting and clearing bits.
//  Return Type: BOOL
//      TRUE(1)         command code audit status was changed
//      FALSE(0)        command code audit status was not changed
BOOL CommandAuditSet(TPM_CC commandCode  // IN: command code
);

//*** CommandAuditClear()
// This function will CLEAR the audit flag for a command. It will not CLEAR the
// audit flag for TPM_CC_SetCommandCodeAuditStatus().
//
// This function is only used by TPM2_SetCommandCodeAuditStatus().
//
// The actions in TPM2_SetCommandCodeAuditStatus() are expected to cause the
// changes to be saved to NV after it is setting and clearing bits.
//  Return Type: BOOL
//      TRUE(1)         command code audit status was changed
//      FALSE(0)        command code audit status was not changed
BOOL CommandAuditClear(TPM_CC commandCode  // IN: command code
);

//*** CommandAuditIsRequired()
// This function indicates if the audit flag is SET for a command.
//  Return Type: BOOL
//      TRUE(1)         command is audited
//      FALSE(0)        command is not audited
BOOL CommandAuditIsRequired(COMMAND_INDEX commandIndex  // IN: command index
);

//*** CommandAuditCapGetCCList()
// This function returns a list of commands that have their audit bit SET.
//
// The list starts at the input commandCode.
//  Return Type: TPMI_YES_NO
//      YES         if there are more command code available
//      NO          all the available command code has been returned
TPMI_YES_NO
CommandAuditCapGetCCList(TPM_CC   commandCode,  // IN: start command code
                         UINT32   count,        // IN: count of returned TPM_CC
                         TPML_CC* commandList   // OUT: list of TPM_CC
);

//*** CommandAuditCapGetOneCC()
// This function returns true if a command has its audit bit set.
BOOL CommandAuditCapGetOneCC(TPM_CC commandCode  // IN: command code
);

//*** CommandAuditGetDigest
// This command is used to create a digest of the commands being audited. The
// commands are processed in ascending numeric order with a list of TPM_CC being
// added to a hash. This operates as if all the audited command codes were
// concatenated and then hashed.
void CommandAuditGetDigest(TPM2B_DIGEST* digest  // OUT: command digest
);

#endif  // _COMMAND_AUDIT_FP_H_
