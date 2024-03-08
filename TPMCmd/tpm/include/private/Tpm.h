// Root header file for building any TPM.lib code

#ifndef _TPM_H_
#define _TPM_H_
// TODO_RENAME_INC_FOLDER: public refers to the TPM_CoreLib public headers
#include <public/tpm_public.h>

#include "TpmAlgorithmDefines.h"
#include "LibSupport.h"        // Types from the library. These need to come before
                               // Global.h because some of the structures in
                               // that file depend on the structures used by the
                               // cryptographic libraries.
#include "GpMacros.h"          // Define additional macros
#include "Global.h"            // Define other TPM types
#include "InternalRoutines.h"  // Function prototypes

#endif  // _TPM_H_
