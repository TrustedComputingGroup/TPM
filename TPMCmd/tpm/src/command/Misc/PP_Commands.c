#include "Tpm.h"
#include "PP_Commands_fp.h"

#if CC_PP_Commands  // Conditional expansion of this file

/*(See part 3 specification)
// This command is used to determine which commands require assertion of
// Physical Presence in addition to platformAuth/platformPolicy.
*/
TPM_RC
TPM2_PP_Commands(PP_Commands_In* in  // IN: input parameter list
)
{
    UINT32 i;

    // The command needs NV update.  Check if NV is available.
    // A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE error may be returned at
    // this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Internal Data Update

    // Process set list
    for(i = 0; i < in->setList.count; i++)
        // If command is implemented, set it as PP required.  If the input
        // command is not a PP command, it will be ignored at
        // PhysicalPresenceCommandSet().
        // Note: PhysicalPresenceCommandSet() checks if the command is implemented.
        PhysicalPresenceCommandSet(in->setList.commandCodes[i]);

    // Process clear list
    for(i = 0; i < in->clearList.count; i++)
        // If command is implemented, clear it as PP required.  If the input
        // command is not a PP command, it will be ignored at
        // PhysicalPresenceCommandClear().  If the input command is
        // TPM2_PP_Commands, it will be ignored as well
        PhysicalPresenceCommandClear(in->clearList.commandCodes[i]);

    // Save the change of PP list
    NV_SYNC_PERSISTENT(ppList);

    return TPM_RC_SUCCESS;
}

#endif  // CC_PP_Commands