//** Introduction
// This file contains the headers necessary to build the tpm big num library.
// TODO_RENAME_INC_FOLDER: public refers to the TPM_CoreLib public headers
#include <public/tpm_public.h>
#include <public/prototypes/TpmFail_fp.h>
// TODO_RENAME_INC_FOLDER: private refers to the TPM_CoreLib private(protected) headers
#include <public/TpmAlgorithmDefines.h>
#include <public/GpMacros.h>  // required for TpmFail_fp.h
#include <public/Capabilities.h>
#include <public/TpmTypes.h>  // requires capabilities & GpMacros
#include <TpmBigNum/TpmToTpmBigNumMath.h>
#include "BnSupport_Interface.h"
#include "BnConvert_fp.h"
#include "BnMemory_fp.h"
#include "BnMath_fp.h"
#include "BnUtil_fp.h"
#include <MathLibraryInterface.h>