//** Includes and Local Values

#define _CRT_RAND_S
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "Platform.h"

#ifdef _MSC_VER
#  include <process.h>
#else
#  include <unistd.h>
#endif

// This is the last 32-bits of hardware entropy produced. We have to check to
// see that two consecutive 32-bit values are not the same because
// according to FIPS 140-2, annex C:
//
// "If each call to an RNG produces blocks of n bits (where n > 15), the first
// n-bit block generated after power-up, initialization, or reset shall not be
// used, but shall be saved for comparison with the next n-bit block to be
// generated. Each subsequent generation of an n-bit block shall be compared with
// the previously generated block. The test shall fail if any two compared n-bit
// blocks are equal."
extern uint32_t lastEntropy;

//** Functions

//*** rand32()
// Local function to get a 32-bit random number
static uint32_t rand32(void)
{
    uint32_t rndNum = rand();
#if RAND_MAX < UINT16_MAX
    // If the maximum value of the random number is a 15-bit number, then shift it up
    // 15 bits, get 15 more bits, shift that up 2 and then XOR in another value to get
    // a full 32 bits.
    rndNum = (rndNum << 15) ^ rand();
    rndNum = (rndNum << 2) ^ rand();
#elif RAND_MAX == UINT16_MAX
    // If the maximum size is 16-bits, shift it and add another 16 bits
    rndNum = (rndNum << 16) ^ rand();
#elif RAND_MAX < UINT32_MAX
    // If 31 bits, then shift 1 and include another random value to get the extra bit
    rndNum = (rndNum << 1) ^ rand();
#endif
    return rndNum;
}

//*** _plat__GetEntropy()
// This function is used to get available hardware entropy. In a hardware
// implementation of this function, there would be no call to the system
// to get entropy.
//  Return Type: int32_t
//  < 0        hardware failure of the entropy generator, this is sticky
// >= 0        the returned amount of entropy (bytes)
//
LIB_EXPORT int32_t _plat__GetEntropy(unsigned char* entropy,  // output buffer
                                     uint32_t       amount    // amount requested
)
{
    uint32_t rndNum;
    int32_t  ret;
    //
    if(amount == 0)
    {
        // Seed the platform entropy source if the entropy source is software. There
        // is no reason to put a guard macro (#if or #ifdef) around this code because
        // this code would not be here if someone was changing it for a system with
        // actual hardware.
        //
        // NOTE 1: The following command does not provide proper cryptographic
        // entropy. Its primary purpose to make sure that different instances of the
        // simulator, possibly started by a script on the same machine, are seeded
        // differently. Vendors of the actual TPMs need to ensure availability of
        // proper entropy using their platform-specific means.
        //
        // NOTE 2: In debug builds by default the reference implementation will seed
        // its RNG deterministically (without using any platform provided randomness).
        // See the USE_DEBUG_RNG macro and DRBG_GetEntropy() function.
#ifdef _MSC_VER
        srand((unsigned)_plat__RealTime() ^ _getpid());
#else
        srand((unsigned)_plat__RealTime() ^ getpid());
#endif
        lastEntropy = rand32();
        ret         = 0;
    }
    else
    {
        rndNum = rand32();
        if(rndNum == lastEntropy)
        {
            ret = -1;
        }
        else
        {
            lastEntropy = rndNum;
            // Each process will have its random number generator initialized
            // according to the process id and the initialization time. This is not a
            // lot of entropy so, to add a bit more, XOR the current time value into
            // the returned entropy value.
            // NOTE: the reason for including the time here rather than have it in
            // in the value assigned to lastEntropy is that rand() could be broken and
            // using the time would in the lastEntropy value would hide this.
            rndNum ^= (uint32_t)_plat__RealTime();

            // Only provide entropy 32 bits at a time to test the ability
            // of the caller to deal with partial results.
            ret = MIN(amount, sizeof(rndNum));
            memcpy(entropy, &rndNum, ret);
        }
    }
    return ret;
}