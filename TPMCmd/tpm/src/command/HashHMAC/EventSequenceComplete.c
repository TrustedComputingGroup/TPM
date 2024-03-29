#include "Tpm.h"
#include "EventSequenceComplete_fp.h"

#if CC_EventSequenceComplete  // Conditional expansion of this file

/*(See part 3 specification)
  Complete an event sequence and flush the object.
*/
//  Return Type: TPM_RC
//      TPM_RC_LOCALITY     PCR extension is not allowed at the current locality
//      TPM_RC_MODE         input handle is not a valid event sequence object
TPM_RC
TPM2_EventSequenceComplete(
    EventSequenceComplete_In*  in,  // IN: input parameter list
    EventSequenceComplete_Out* out  // OUT: output parameter list
)
{
    HASH_OBJECT* hashObject;
    UINT32       i;
    TPM_ALG_ID   hashAlg;
    // Input validation
    // get the event sequence object pointer
    hashObject = (HASH_OBJECT*)HandleToObject(in->sequenceHandle);

    // input handle must reference an event sequence object
    if(hashObject->attributes.eventSeq != SET)
        return TPM_RCS_MODE + RC_EventSequenceComplete_sequenceHandle;

    // see if a PCR extend is requested in call
    if(in->pcrHandle != TPM_RH_NULL)
    {
        // see if extend of the PCR is allowed at the locality of the command,
        if(!PCRIsExtendAllowed(in->pcrHandle))
            return TPM_RC_LOCALITY;
        // if an extend is going to take place, then check to see if there has
        // been an orderly shutdown. If so, and the selected PCR is one of the
        // state saved PCR, then the orderly state has to change. The orderly state
        // does not change for PCR that are not preserved.
        // NOTE: This doesn't just check for Shutdown(STATE) because the orderly
        // state will have to change if this is a state-saved PCR regardless
        // of the current state. This is because a subsequent Shutdown(STATE) will
        // check to see if there was an orderly shutdown and not do anything if
        // there was. So, this must indicate that a future Shutdown(STATE) has
        // something to do.
        if(PCRIsStateSaved(in->pcrHandle))
            RETURN_IF_ORDERLY;
    }
    // Command Output
    out->results.count = 0;

    for(i = 0; i < HASH_COUNT; i++)
    {
        hashAlg = CryptHashGetAlgByIndex(i);
        // Update last piece of data
        CryptDigestUpdate2B(&hashObject->state.hashState[i], &in->buffer.b);
        // Complete hash
        out->results.digests[out->results.count].hashAlg = hashAlg;
        CryptHashEnd(&hashObject->state.hashState[i],
                     CryptHashGetDigestSize(hashAlg),
                     (BYTE*)&out->results.digests[out->results.count].digest);
        // Extend PCR
        if(in->pcrHandle != TPM_RH_NULL)
            PCRExtend(in->pcrHandle,
                      hashAlg,
                      CryptHashGetDigestSize(hashAlg),
                      (BYTE*)&out->results.digests[out->results.count].digest);
        out->results.count++;
    }
    // Internal Data Update
    // mark sequence object as evict so it will be flushed on the way out
    hashObject->attributes.evict = SET;

    return TPM_RC_SUCCESS;
}

#endif  // CC_EventSequenceComplete