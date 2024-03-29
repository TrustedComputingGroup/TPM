#include "Tpm.h"
#include "PCR_SetAuthPolicy_fp.h"

#if CC_PCR_SetAuthPolicy  // Conditional expansion of this file

/*(See part 3 specification)
// Set authPolicy to a group of PCR
*/
//  Return Type: TPM_RC
//      TPM_RC_SIZE                 size of 'authPolicy' is not the size of a digest
//                                  produced by 'policyDigest'
//      TPM_RC_VALUE                PCR referenced by 'pcrNum' is not a member
//                                  of a PCR policy group
TPM_RC
TPM2_PCR_SetAuthPolicy(PCR_SetAuthPolicy_In* in  // IN: input parameter list
)
{
    UINT32 groupIndex;

    // The command needs NV update.  Check if NV is available.
    // A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE error may be returned at
    // this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Input Validation:

    // Check the authPolicy consistent with hash algorithm
    if(in->authPolicy.t.size != CryptHashGetDigestSize(in->hashAlg))
        return TPM_RCS_SIZE + RC_PCR_SetAuthPolicy_authPolicy;

    // If PCR does not belong to a policy group, return TPM_RC_VALUE
    if(!PCRBelongsPolicyGroup(in->pcrNum, &groupIndex))
        return TPM_RCS_VALUE + RC_PCR_SetAuthPolicy_pcrNum;

    // Internal Data Update

    // Set PCR policy
    gp.pcrPolicies.hashAlg[groupIndex] = in->hashAlg;
    gp.pcrPolicies.policy[groupIndex]  = in->authPolicy;

    // Save new policy to NV
    NV_SYNC_PERSISTENT(pcrPolicies);

    return TPM_RC_SUCCESS;
}

#endif  // CC_PCR_SetAuthPolicy