#include "Tpm.h"
#include "SetPrimaryPolicy_fp.h"

#if CC_SetPrimaryPolicy  // Conditional expansion of this file

/*(See part 3 specification)
// Set a hierarchy policy
*/
//  Return Type: TPM_RC
//      TPM_RC_SIZE           size of input authPolicy is not consistent with
//                            input hash algorithm
TPM_RC
TPM2_SetPrimaryPolicy(SetPrimaryPolicy_In* in  // IN: input parameter list
)
{
    // Input Validation

    // Check the authPolicy consistent with hash algorithm. If the policy size is
    // zero, then the algorithm is required to be TPM_ALG_NULL
    if(in->authPolicy.t.size != CryptHashGetDigestSize(in->hashAlg))
        return TPM_RCS_SIZE + RC_SetPrimaryPolicy_authPolicy;

    // The command need NV update for OWNER and ENDORSEMENT hierarchy, and
    // might need orderlyState update for PLATFROM hierarchy.
    // Check if NV is available.  A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE
    // error may be returned at this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Internal Data Update

    // Set hierarchy policy
    switch(in->authHandle)
    {
        case TPM_RH_OWNER:
            gp.ownerAlg    = in->hashAlg;
            gp.ownerPolicy = in->authPolicy;
            NV_SYNC_PERSISTENT(ownerAlg);
            NV_SYNC_PERSISTENT(ownerPolicy);
            break;
        case TPM_RH_ENDORSEMENT:
            gp.endorsementAlg    = in->hashAlg;
            gp.endorsementPolicy = in->authPolicy;
            NV_SYNC_PERSISTENT(endorsementAlg);
            NV_SYNC_PERSISTENT(endorsementPolicy);
            break;
        case TPM_RH_PLATFORM:
            gc.platformAlg    = in->hashAlg;
            gc.platformPolicy = in->authPolicy;
            // need to update orderly state
            g_clearOrderly = TRUE;
            break;
        case TPM_RH_LOCKOUT:
            gp.lockoutAlg    = in->hashAlg;
            gp.lockoutPolicy = in->authPolicy;
            NV_SYNC_PERSISTENT(lockoutAlg);
            NV_SYNC_PERSISTENT(lockoutPolicy);
            break;

#  if ACT_SUPPORT
#    define SET_ACT_POLICY(N)                       \
        case TPM_RH_ACT_##N:                        \
            go.ACT_##N.hashAlg    = in->hashAlg;    \
            go.ACT_##N.authPolicy = in->authPolicy; \
            g_clearOrderly        = TRUE;           \
            break;

            FOR_EACH_ACT(SET_ACT_POLICY)
#  endif  // ACT_SUPPORT

        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }

    return TPM_RC_SUCCESS;
}

#endif  // CC_SetPrimaryPolicy