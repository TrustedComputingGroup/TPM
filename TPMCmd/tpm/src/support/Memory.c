//** Description
// This file contains a set of miscellaneous memory manipulation routines. Many
// of the functions have the same semantics as functions defined in string.h.
// Those functions are not used directly in the TPM because they are not 'safe'
//
// This version uses string.h after adding guards.  This is because the math
// libraries invariably use those functions so it is not practical to prevent
// those library functions from being pulled into the build.

//** Includes and Data Definitions
#include "Tpm.h"
#include "Memory_fp.h"

//** Functions

//*** MemoryCopy()
// This is an alias for memmove. This is used in place of memcpy because
// some of the moves may overlap and rather than try to make sure that
// memmove is used when necessary, it is always used.
void MemoryCopy(void* dest, const void* src, int sSize)
{
    if(dest != src)
        memmove(dest, src, sSize);
}

//*** MemoryEqual()
// This function indicates if two buffers have the same values in the indicated
// number of bytes.
//  Return Type: BOOL
//      TRUE(1)         all octets are the same
//      FALSE(0)        all octets are not the same
BOOL MemoryEqual(const void*  buffer1,  // IN: compare buffer1
                 const void*  buffer2,  // IN: compare buffer2
                 unsigned int size      // IN: size of bytes being compared
)
{
    BYTE        equal = 0;
    const BYTE* b1    = (BYTE*)buffer1;
    const BYTE* b2    = (BYTE*)buffer2;
    //
    // Compare all bytes so that there is no leakage of information
    // due to timing differences.
    for(; size > 0; size--)
        equal |= (*b1++ ^ *b2++);
    return (equal == 0);
}

//*** MemoryCopy2B()
// This function copies a TPM2B. This can be used when the TPM2B types are
// the same or different.
//
// This function returns the number of octets in the data buffer of the TPM2B.
LIB_EXPORT INT16 MemoryCopy2B(TPM2B*       dest,    // OUT: receiving TPM2B
                              const TPM2B* source,  // IN: source TPM2B
                              unsigned int dSize  // IN: size of the receiving buffer
)
{
    pAssert(dest != NULL);
    if(source == NULL)
        dest->size = 0;
    else
    {
        pAssert(source->size <= dSize);
        MemoryCopy(dest->buffer, source->buffer, source->size);
        dest->size = source->size;
    }
    return dest->size;
}

//*** MemoryConcat2B()
// This function will concatenate the buffer contents of a TPM2B to
// the buffer contents of another TPM2B and adjust the size accordingly
//      ('a' := ('a' | 'b')).
void MemoryConcat2B(
    TPM2B*       aInOut,   // IN/OUT: destination 2B
    TPM2B*       bIn,      // IN: second 2B
    unsigned int aMaxSize  // IN: The size of aInOut.buffer (max values for
                           //     aInOut.size)
)
{
    pAssert(bIn->size <= aMaxSize - aInOut->size);
    MemoryCopy(&aInOut->buffer[aInOut->size], &bIn->buffer, bIn->size);
    aInOut->size = aInOut->size + bIn->size;
    return;
}

//*** MemoryEqual2B()
// This function will compare two TPM2B structures. To be equal, they
// need to be the same size and the buffer contexts need to be the same
// in all octets.
//  Return Type: BOOL
//      TRUE(1)         size and buffer contents are the same
//      FALSE(0)        size or buffer contents are not the same
BOOL MemoryEqual2B(const TPM2B* aIn,  // IN: compare value
                   const TPM2B* bIn   // IN: compare value
)
{
    if(aIn->size != bIn->size)
        return FALSE;
    return MemoryEqual(aIn->buffer, bIn->buffer, aIn->size);
}

//*** MemorySet()
// This function will set all the octets in the specified memory range to
// the specified octet value.
// Note: A previous version had an additional parameter (dSize) that was
// intended to make sure that the destination would not be overrun. The
// problem is that, in use, all that was happening was that the value of
// size was used for dSize so there was no benefit in the extra parameter.
void MemorySet(void* dest, int value, size_t size)
{
    memset(dest, value, size);
}

//*** MemoryPad2B()
// Function to pad a TPM2B with zeros and adjust the size.
void MemoryPad2B(TPM2B* b, UINT16 newSize)
{
    MemorySet(&b->buffer[b->size], 0, newSize - b->size);
    b->size = newSize;
}

//*** Uint16ToByteArray()
// Function to write an integer to a byte array
void Uint16ToByteArray(UINT16 i, BYTE* a)
{
    a[1] = (BYTE)(i);
    i >>= 8;
    a[0] = (BYTE)(i);
}

//*** Uint32ToByteArray()
// Function to write an integer to a byte array
void Uint32ToByteArray(UINT32 i, BYTE* a)
{
    a[3] = (BYTE)(i);
    i >>= 8;
    a[2] = (BYTE)(i);
    i >>= 8;
    a[1] = (BYTE)(i);
    i >>= 8;
    a[0] = (BYTE)(i);
}

//*** Uint64ToByteArray()
// Function to write an integer to a byte array
void Uint64ToByteArray(UINT64 i, BYTE* a)
{
    a[7] = (BYTE)(i);
    i >>= 8;
    a[6] = (BYTE)(i);
    i >>= 8;
    a[5] = (BYTE)(i);
    i >>= 8;
    a[4] = (BYTE)(i);
    i >>= 8;
    a[3] = (BYTE)(i);
    i >>= 8;
    a[2] = (BYTE)(i);
    i >>= 8;
    a[1] = (BYTE)(i);
    i >>= 8;
    a[0] = (BYTE)(i);
}

//*** ByteArrayToUint8()
// Function to write a UINT8 to a byte array. This is included for completeness
// and to allow certain macro expansions
UINT8
ByteArrayToUint8(BYTE* a)
{
    return *a;
}

//*** ByteArrayToUint16()
// Function to write an integer to a byte array
UINT16
ByteArrayToUint16(BYTE* a)
{
    return ((UINT16)a[0] << 8) + a[1];
}

//*** ByteArrayToUint32()
// Function to write an integer to a byte array
UINT32
ByteArrayToUint32(BYTE* a)
{
    return (UINT32)((((((UINT32)a[0] << 8) + a[1]) << 8) + (UINT32)a[2]) << 8) + a[3];
}

//*** ByteArrayToUint64()
// Function to write an integer to a byte array
UINT64
ByteArrayToUint64(BYTE* a)
{
    return (((UINT64)BYTE_ARRAY_TO_UINT32(a)) << 32) + BYTE_ARRAY_TO_UINT32(&a[4]);
}