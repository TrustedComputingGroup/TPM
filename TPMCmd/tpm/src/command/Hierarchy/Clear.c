#include "Tpm.h"
#include "Clear_fp.h"

#if CC_Clear  // Conditional expansion of this file

/*(See part 3 specification)
// Clear owner
*/
//  Return Type: TPM_RC
//      TPM_RC_DISABLED             Clear command has been disabled
TPM_RC
TPM2_Clear(Clear_In* in  // IN: input parameter list
)
{
    // Input parameter is not reference in command action
    NOT_REFERENCED(in);

    // The command needs NV update.  Check if NV is available.
    // A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE error may be returned at
    // this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Input Validation

    // If Clear command is disabled, return an error
    if(gp.disableClear)
        return TPM_RC_DISABLED;

    // Internal Data Update

    // Reset storage hierarchy seed from RNG
    CryptRandomGenerate(sizeof(gp.SPSeed.t.buffer), gp.SPSeed.t.buffer);

    // Create new shProof and ehProof value from RNG
    CryptRandomGenerate(sizeof(gp.shProof.t.buffer), gp.shProof.t.buffer);
    CryptRandomGenerate(sizeof(gp.ehProof.t.buffer), gp.ehProof.t.buffer);

    // Enable storage and endorsement hierarchy
    gc.shEnable = gc.ehEnable = TRUE;

    // set the authValue buffers to zero
    MemorySet(&gp.ownerAuth, 0, sizeof(gp.ownerAuth));
    MemorySet(&gp.endorsementAuth, 0, sizeof(gp.endorsementAuth));
    MemorySet(&gp.lockoutAuth, 0, sizeof(gp.lockoutAuth));

    // Set storage, endorsement, and lockout authPolicy to null
    gp.ownerAlg = gp.endorsementAlg = gp.lockoutAlg = TPM_ALG_NULL;
    MemorySet(&gp.ownerPolicy, 0, sizeof(gp.ownerPolicy));
    MemorySet(&gp.endorsementPolicy, 0, sizeof(gp.endorsementPolicy));
    MemorySet(&gp.lockoutPolicy, 0, sizeof(gp.lockoutPolicy));

    // Flush loaded object in storage and endorsement hierarchy
    ObjectFlushHierarchy(TPM_RH_OWNER);
    ObjectFlushHierarchy(TPM_RH_ENDORSEMENT);

    // Flush owner and endorsement object and owner index in NV
    NvFlushHierarchy(TPM_RH_OWNER);
    NvFlushHierarchy(TPM_RH_ENDORSEMENT);

    // Initialize dictionary attack parameters
    DAPreInstall_Init();

    // Reset clock
    go.clock     = 0;
    go.clockSafe = YES;
    NvWrite(NV_ORDERLY_DATA, sizeof(ORDERLY_DATA), &go);

    // Reset counters
    gp.resetCount = gr.restartCount = gr.clearCount = 0;
    gp.auditCounter                                 = 0;

    // Save persistent data changes to NV
    // Note: since there are so many changes to the persistent data structure, the
    // entire PERSISTENT_DATA structure is written as a unit
    NvWrite(NV_PERSISTENT_DATA, sizeof(PERSISTENT_DATA), &gp);

    // Reset the PCR authValues (this does not change the PCRs)
    PCR_ClearAuth();

    // Bump the PCR counter
    PCRChanged(0);

    // orderly state should be cleared because of the update to state clear data
    g_clearOrderly = TRUE;

    return TPM_RC_SUCCESS;
}

#endif  // CC_Clear