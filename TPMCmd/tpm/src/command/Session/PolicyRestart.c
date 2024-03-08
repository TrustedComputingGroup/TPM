#include "Tpm.h"
#include "PolicyRestart_fp.h"

#if CC_PolicyRestart  // Conditional expansion of this file

/*(See part 3 specification)
// Restore a policy session to its initial state
*/
TPM_RC
TPM2_PolicyRestart(PolicyRestart_In* in  // IN: input parameter list
)
{
    // Initialize policy session data
    SessionResetPolicyData(SessionGet(in->sessionHandle));

    return TPM_RC_SUCCESS;
}

#endif  // CC_PolicyRestart