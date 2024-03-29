#include "Tpm.h"
#include "NV_ReadPublic2_fp.h"

#if CC_NV_ReadPublic2  // Conditional expansion of this file

/*(See part 3 specification)
// Read the public information of a NV index
*/
TPM_RC
TPM2_NV_ReadPublic2(NV_ReadPublic2_In*  in,  // IN: input parameter list
                    NV_ReadPublic2_Out* out  // OUT: output parameter list
)
{
    TPM_RC    result;
    NV_INDEX* nvIndex;

    nvIndex = NvGetIndexInfo(in->nvIndex, NULL);

    // Command Output

    // The reference code stores its NV indices in the legacy form, because
    // it doesn't support any extended attributes.
    // Translate the legacy form to the general form.
    result = NvPublic2FromNvPublic(&nvIndex->publicArea, &out->nvPublic.nvPublic2);
    if(result != TPM_RC_SUCCESS)
    {
        return RcSafeAddToResult(result, RC_NV_ReadPublic2_nvIndex);
    }

    // Compute NV name
    NvGetIndexName(nvIndex, &out->nvName);

    return TPM_RC_SUCCESS;
}

#endif  // CC_NV_ReadPublic2