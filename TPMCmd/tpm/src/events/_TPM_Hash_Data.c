#include "Tpm.h"

// This function is called to process a _TPM_Hash_Data indication.
LIB_EXPORT void _TPM_Hash_Data(uint32_t dataSize,   // IN: size of data to be extend
                               unsigned char* data  // IN: data buffer
)
{
    UINT32       i;
    HASH_OBJECT* hashObject;
    TPMI_DH_PCR  pcrHandle = TPMIsStarted() ? PCR_FIRST + DRTM_PCR
                                            : PCR_FIRST + HCRTM_PCR;

    // If there is no DRTM sequence object, then _TPM_Hash_Start
    // was not called so this function returns without doing
    // anything.
    if(g_DRTMHandle == TPM_RH_UNASSIGNED)
        return;

    hashObject = (HASH_OBJECT*)HandleToObject(g_DRTMHandle);
    pAssert(hashObject->attributes.eventSeq);

    // For each of the implemented hash algorithms, update the digest with the
    // data provided.
    for(i = 0; i < HASH_COUNT; i++)
    {
        // make sure that the PCR is implemented for this algorithm
        if(PcrIsAllocated(pcrHandle, hashObject->state.hashState[i].hashAlg))
            // Update sequence object
            CryptDigestUpdate(&hashObject->state.hashState[i], dataSize, data);
    }

    return;
}