#include "Tpm.h"
#include "HierarchyControl_fp.h"

#if CC_HierarchyControl  // Conditional expansion of this file

/*(See part 3 specification)
// Enable or disable use of a hierarchy
*/
//  Return Type: TPM_RC
//      TPM_RC_AUTH_TYPE        'authHandle' is not applicable to 'hierarchy' in its
//                              current state
TPM_RC
TPM2_HierarchyControl(HierarchyControl_In* in  // IN: input parameter list
)
{
    BOOL  select   = (in->state == YES);
    BOOL* selected = NULL;

    // Input Validation
    switch(in->enable)
    {
        // Platform hierarchy has to be disabled by PlatformAuth
        // If the platform hierarchy has already been disabled, only a reboot
        // can enable it again
        case TPM_RH_PLATFORM:
        case TPM_RH_PLATFORM_NV:
            if(in->authHandle != TPM_RH_PLATFORM)
                return TPM_RC_AUTH_TYPE;
            break;

        // ShEnable may be disabled if PlatformAuth/PlatformPolicy or
        // OwnerAuth/OwnerPolicy is provided.  If ShEnable is disabled, then it
        // may only be enabled if PlatformAuth/PlatformPolicy is provided.
        case TPM_RH_OWNER:
            if(in->authHandle != TPM_RH_PLATFORM && in->authHandle != TPM_RH_OWNER)
                return TPM_RC_AUTH_TYPE;
            if(gc.shEnable == FALSE && in->state == YES
               && in->authHandle != TPM_RH_PLATFORM)
                return TPM_RC_AUTH_TYPE;
            break;

        // EhEnable may be disabled if either PlatformAuth/PlatformPolicy or
        // EndosementAuth/EndorsementPolicy is provided.  If EhEnable is disabled,
        // then it may only be enabled if PlatformAuth/PlatformPolicy is
        // provided.
        case TPM_RH_ENDORSEMENT:
            if(in->authHandle != TPM_RH_PLATFORM
               && in->authHandle != TPM_RH_ENDORSEMENT)
                return TPM_RC_AUTH_TYPE;
            if(gc.ehEnable == FALSE && in->state == YES
               && in->authHandle != TPM_RH_PLATFORM)
                return TPM_RC_AUTH_TYPE;
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }

    // Internal Data Update

    // Enable or disable the selected hierarchy
    // Note: the authorization processing for this command may keep these
    // command actions from being executed. For example, if phEnable is
    // CLEAR, then platformAuth cannot be used for authorization. This
    // means that would not be possible to use platformAuth to change the
    // state of phEnable from CLEAR to SET.
    // If it is decided that platformPolicy can still be used when phEnable
    // is CLEAR, then this code could SET phEnable when proper platform
    // policy is provided.
    switch(in->enable)
    {
        case TPM_RH_OWNER:
            selected = &gc.shEnable;
            break;
        case TPM_RH_ENDORSEMENT:
            selected = &gc.ehEnable;
            break;
        case TPM_RH_PLATFORM:
            selected = &g_phEnable;
            break;
        case TPM_RH_PLATFORM_NV:
            selected = &gc.phEnableNV;
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    if(selected != NULL && *selected != select)
    {
        // Before changing the internal state, make sure that NV is available.
        // Only need to update NV if changing the orderly state
        RETURN_IF_ORDERLY;

        // state is changing and NV is available so modify
        *selected = select;
        // If a hierarchy was just disabled, flush it
        if(select == CLEAR && in->enable != TPM_RH_PLATFORM_NV)
            // Flush hierarchy
            ObjectFlushHierarchy(in->enable);

        // orderly state should be cleared because of the update to state clear data
        // This gets processed in ExecuteCommand() on the way out.
        g_clearOrderly = TRUE;
    }
    return TPM_RC_SUCCESS;
}

#endif  // CC_HierarchyControl