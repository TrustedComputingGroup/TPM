/*(Auto-generated)
 *  Created by TpmPrototypes; Version 3.0 July 18, 2017
 *  Date: Apr  2, 2019  Time: 03:18:00PM
 */

#ifndef _TPM_FAIL_FP_H_
#define _TPM_FAIL_FP_H_

//*** SetForceFailureMode()
// This function is called by the simulator to enable failure mode testing.
#if SIMULATION
LIB_EXPORT void SetForceFailureMode(void);
#endif

//*** TpmFail()
// This function is called by TPM.lib when a failure occurs. It will set up the
// failure values to be returned on TPM2_GetTestResult().
NORETURN void TpmFail(
#if FAIL_TRACE
    const char* function,
    int         line,
#else
    uint64_t locationCode,
#endif
    int failureCode);

//*** TpmFailureMode(
// This function is called by the interface code when the platform is in failure
// mode.
void TpmFailureMode(uint32_t        inRequestSize,    // IN: command buffer size
                    unsigned char*  inRequest,        // IN: command buffer
                    uint32_t*       outResponseSize,  // OUT: response buffer size
                    unsigned char** outResponse       // OUT: response buffer
);

//*** UnmarshalFail()
// This is a stub that is used to catch an attempt to unmarshal an entry
// that is not defined. Don't ever expect this to be called but...
void UnmarshalFail(void* type, BYTE** buffer, INT32* size);

#endif  // _TPM_FAIL_FP_H_
