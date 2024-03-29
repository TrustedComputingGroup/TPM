/*(Auto-generated)
 *  Created by TpmPrototypes; Version 3.0 July 18, 2017
 *  Date: Aug 30, 2019  Time: 02:11:54PM
 */

#ifndef _TPM_ASN1_FP_H_
#define _TPM_ASN1_FP_H_

//*** ASN1UnmarshalContextInitialize()
// Function does standard initialization of a context.
//  Return Type: BOOL
//      TRUE(1)     success
//      FALSE(0)    failure
BOOL ASN1UnmarshalContextInitialize(
    ASN1UnmarshalContext* ctx, INT16 size, BYTE* buffer);

//***ASN1DecodeLength()
// This function extracts the length of an element from 'buffer' starting at 'offset'.
// Return Type: UINT16
//      >=0         the extracted length
//      <0          an error
INT16
ASN1DecodeLength(ASN1UnmarshalContext* ctx);

//***ASN1NextTag()
// This function extracts the next type from 'buffer' starting at 'offset'.
// It advances 'offset' as it parses the type and the length of the type. It returns
// the length of the type. On return, the 'length' octets starting at 'offset' are the
// octets of the type.
// Return Type: UINT
//     >=0          the number of octets in 'type'
//     <0           an error
INT16
ASN1NextTag(ASN1UnmarshalContext* ctx);

//*** ASN1GetBitStringValue()
// Try to parse a bit string of up to 32 bits from a value that is expected to be
// a bit string. The bit string is left justified so that the MSb of the input is
// the MSb of the returned value.
// If there is a general parsing error, the context->size is set to -1.
//  Return Type: BOOL
//      TRUE(1)     success
//      FALSE(0)    failure
BOOL ASN1GetBitStringValue(ASN1UnmarshalContext* ctx, UINT32* val);

//*** ASN1InitialializeMarshalContext()
// This creates a structure for handling marshaling of an ASN.1 formatted data
// structure.
void ASN1InitialializeMarshalContext(
    ASN1MarshalContext* ctx, INT16 length, BYTE* buffer);

//*** ASN1StartMarshalContext()
// This starts a new constructed element. It is constructed on 'top' of the value
// that was previously placed in the structure.
void ASN1StartMarshalContext(ASN1MarshalContext* ctx);

//*** ASN1EndMarshalContext()
// This function restores the end pointer for an encapsulating structure.
//  Return Type: INT16
//      > 0             the size of the encapsulated structure that was just ended
//      <= 0            an error
INT16
ASN1EndMarshalContext(ASN1MarshalContext* ctx);

//***ASN1EndEncapsulation()
// This function puts a tag and length in the buffer. In this function, an embedded
// BIT_STRING is assumed to be a collection of octets. To indicate that all bits
// are used, a byte of zero is prepended. If a raw bit-string is needed, a new
// function like ASN1PushInteger() would be needed.
//  Return Type: INT16
//      > 0         number of octets in the encapsulation
//      == 0        failure
UINT16
ASN1EndEncapsulation(ASN1MarshalContext* ctx, BYTE tag);

//*** ASN1PushByte()
BOOL ASN1PushByte(ASN1MarshalContext* ctx, BYTE b);

//*** ASN1PushBytes()
// Push some raw bytes onto the buffer. 'count' cannot be zero.
//  Return Type: IN16
//      > 0             count bytes
//      == 0            failure unless count was zero
INT16
ASN1PushBytes(ASN1MarshalContext* ctx, INT16 count, const BYTE* buffer);

//*** ASN1PushNull()
//  Return Type: IN16
//      > 0             count bytes
//      == 0            failure unless count was zero
INT16
ASN1PushNull(ASN1MarshalContext* ctx);

//*** ASN1PushLength()
// Push a length value. This will only handle length values that fit in an INT16.
//  Return Type: UINT16
//      > 0         number of bytes added
//      == 0        failure
INT16
ASN1PushLength(ASN1MarshalContext* ctx, INT16 len);

//*** ASN1PushTagAndLength()
//  Return Type: INT16
//      > 0         number of bytes added
//      == 0        failure
INT16
ASN1PushTagAndLength(ASN1MarshalContext* ctx, BYTE tag, INT16 length);

//*** ASN1PushTaggedOctetString()
// This function will push a random octet string.
//  Return Type: INT16
//      > 0         number of bytes added
//      == 0        failure
INT16
ASN1PushTaggedOctetString(
    ASN1MarshalContext* ctx, INT16 size, const BYTE* string, BYTE tag);

//*** ASN1PushUINT()
// This function pushes an native-endian integer value. This just changes a
// native-endian integer into a big-endian byte string and calls ASN1PushInteger().
// That function will remove leading zeros and make sure that the number is positive.
//  Return Type: IN16
//      > 0             count bytes
//      == 0            failure unless count was zero
INT16
ASN1PushUINT(ASN1MarshalContext* ctx, UINT32 integer);

//*** ASN1PushInteger
// Push a big-endian integer on the end of the buffer
//  Return Type: UINT16
//      > 0         the number of bytes marshaled for the integer
//      == 0        failure
INT16
ASN1PushInteger(ASN1MarshalContext* ctx,     // IN/OUT: buffer context
                INT16               iLen,    // IN: octets of the integer
                BYTE*               integer  // IN: big-endian integer
);

//*** ASN1PushOID()
// This function is used to add an OID. An OID is 0x06 followed by a byte of size
// followed by size bytes. This is used to avoid having to do anything special in the
// definition of an OID.
//  Return Type: UINT16
//      > 0         the number of bytes marshaled for the integer
//      == 0        failure
INT16
ASN1PushOID(ASN1MarshalContext* ctx, const BYTE* OID);

#endif  // _TPM_ASN1_FP_H_
