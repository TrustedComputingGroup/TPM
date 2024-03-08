#include "Tpm.h"
#include "PCR_Read_fp.h"

#if CC_PCR_Read  // Conditional expansion of this file

/*(See part 3 specification)
// Read a set of PCR
*/
TPM_RC
TPM2_PCR_Read(PCR_Read_In*  in,  // IN: input parameter list
              PCR_Read_Out* out  // OUT: output parameter list
)
{
    // Command Output

    // Call PCR read function.  input pcrSelectionIn parameter could be changed
    // to reflect the actual PCR being returned
    PCRRead(&in->pcrSelectionIn, &out->pcrValues, &out->pcrUpdateCounter);

    out->pcrSelectionOut = in->pcrSelectionIn;

    return TPM_RC_SUCCESS;
}

#endif  // CC_PCR_Read