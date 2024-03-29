//** Introduction
// In some implementations of the TPM, the hardware can provide a secret
// value to the TPM. This secret value is statistically unique to the
// instance of the TPM. Typical uses of this value are to provide
// personalization to the random number generation and as a shared secret
// between the TPM and the manufacturer.

//** Includes
#include "Platform.h"

#if VENDOR_PERMANENT_AUTH_ENABLED == YES

const char notReallyUnique[] = "This is not really a unique value. A real "
                               "unique value should"
                               " be generated by the platform.";

//** _plat__GetUnique()
// This function is used to access the platform-specific vendor unique values.
// This function places the unique value in the provided buffer ('b')
// and returns the number of bytes transferred. The function will not
// copy more data than 'bSize'.
// NOTE: If a platform unique value has unequal distribution of uniqueness
// and 'bSize' is smaller than the size of the unique value, the 'bSize'
// portion with the most uniqueness should be returned.
//
// 'which' indicates the unique value to return:
// 0 = RESERVED, do not use
// 1 = the VENDOR_PERMANENT_AUTH_HANDLE authorization value for this device
LIB_EXPORT uint32_t _plat__GetUnique(uint32_t which,  // which vendor value to return?
                                     uint32_t bSize,  // size of the buffer
                                     unsigned char* b  // output buffer
)
{
    const char* from   = notReallyUnique;
    uint32_t    retVal = 0;

    if(which == 1)
    {
        const size_t uSize =
            sizeof(notReallyUnique) <= bSize ? sizeof(notReallyUnique) : bSize;
        MemoryCopy(b, notReallyUnique, uSize);
    }
    // else fall through to default 0

    return retVal;
}

#endif