#include "Tpm.h"
#include "NV_ReadPublic_fp.h"

#if CC_NV_ReadPublic  // Conditional expansion of this file

/*(See part 3 specification)
// Read the public information of a NV index
*/
TPM_RC
TPM2_NV_ReadPublic(NV_ReadPublic_In*  in,  // IN: input parameter list
                   NV_ReadPublic_Out* out  // OUT: output parameter list
)
{
    NV_INDEX* nvIndex;

    // This command only supports TPM_HT_NV_INDEX-typed NV indices.
    if(HandleGetType(in->nvIndex) != TPM_HT_NV_INDEX)
    {
        return TPM_RCS_HANDLE + RC_NV_ReadPublic_nvIndex;
    }

    nvIndex = NvGetIndexInfo(in->nvIndex, NULL);

    // Command Output

    // Copy index public data to output
    out->nvPublic.nvPublic = nvIndex->publicArea;

    // Compute NV name
    NvGetIndexName(nvIndex, &out->nvName);

    return TPM_RC_SUCCESS;
}

#endif  // CC_NV_ReadPublic