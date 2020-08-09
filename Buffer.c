/* File: Buffer.c

Author: Henry Chuang

Date: 2/7/2020

Purpose: Contains functions manipulating buffers.
Buffers can contain a number of data bytes that can be added or removed.
The functions include buffer initialization, resetting buffers,
checking if a buffer is closed, opening or closing a buffer,
checking if a buffer is full or empty, and adding or removing bytes.
*/

#include "Buffer.h"

/*---------------- BfrInit() ----------------

Purpose:
Initialize a buffer: record the address of the buffer space and size,
reset putIndex and getIndex to zero,
and mark the buffer open.

Parameters: buffer address
            address of the buffer's data space
            buffer capacity in bytes

Return value: none
*/

CPU_VOID BfrInit(Buffer *bfr) {
    BfrReset(bfr);
}

/*---------------- BfrReset() ----------------

Purpose:
Reset the buffer: reset putIndex and getIndex to zero,
and markt the buffer open.

Parameters: buffer address

Return value: none
*/

CPU_VOID BfrReset(Buffer *bfr) {
    bfr->putIndex = bfr->buffer;
    bfr->getIndex = bfr->buffer;
    bfr->closed = FALSE;
}

/*---------------- BfrClosed() ----------------

Purpose:
Test whether a buffer is closed.

Parameters: buffer address

Return value: TRUE if closed; otherwise FALSE
*/

CPU_BOOLEAN BfrClosed(Buffer *bfr) {
    return bfr->closed;
}

/*---------------- BfrClose() ----------------

Purpose:
Mark the buffer closed.

Parameters: buffer address

Return value: none
*/

CPU_VOID BfrClose(Buffer *bfr) {
    bfr->closed = TRUE;
}

/*---------------- BfrOpen() ----------------

Purpose:
Mark the buffer open.

Parameters: buffer address

Return value: none
*/

CPU_VOID BfrOpen(Buffer *bfr) {
    bfr->closed = FALSE;
}

/*---------------- BfrFull() ----------------

Purpose:
Test whether a buffer is full, which is when putIndex >= size.

Parameters: buffer address

Return value: TRUE if full, otherwise FALSE
*/

CPU_BOOLEAN BfrFull(Buffer *bfr) {
    return bfr->putIndex >= bfr->buffer+IOBfrSize;
}

/*---------------- BfrEmpty() ----------------

Purpose:
Test whether a buffer is empty, which is when getIndex >= putIndex.

Parameters: buffer address

Return value: TRUE if empty, otherwise FALSE
*/

CPU_BOOLEAN BfrEmpty(Buffer *bfr) {
    return bfr->getIndex >= bfr->putIndex;
}

/*---------------- BfrAddByte() ----------------

Purpose:
If the buffer is already full, return -1.
Otherwise, add a byte to the buffer at position "putIndex" and increment putIndex by 1.
If the buffer becomes full, mark it closed.

Parameters: buffer address

Return value: the byte added, unless the buffer was full; otherwise -1
*/

CPU_INT08S BfrAddByte(Buffer *bfr, CPU_INT08S theByte) {
    if(BfrFull(bfr))
        return -1;

    *bfr->putIndex++ = theByte;

    if(BfrFull(bfr))
        BfrClose(bfr);
    
    return theByte;
}

/*---------------- BfrNextByte() ----------------

Purpose:
Return the byte from position "getIndex" or return -1 if the buffer is empty.

Parameters: buffer address

Return value: the byte from position "getIndex", unless the buffer was full;
otherwise -1
*/

CPU_INT08S BfrNextByte(Buffer *bfr) {
    if(BfrEmpty(bfr))
        return -1;
    
    return *bfr->getIndex;
}

/*---------------- BfrRemByte() ----------------

Purpose:
If the buffer is already empty, return -1.
Otherwise, return the byte from position "getIndex" and increment "getIndex" by 1.
If the buffer becomes empty, mark it open.

Parameters: buffer address

Return value: the byte from position "getIndex", unless the buffer was full;
otherwise -1
*/

CPU_INT08S BfrRemByte(Buffer *bfr) {
    CPU_INT08S remByte = BfrNextByte(bfr);
    
    if(remByte == -1)
        return -1;
    
    bfr->getIndex++;
    
    if(BfrEmpty(bfr))
        BfrOpen(bfr);
    
    return remByte;
}

CPU_VOID PayloadInit(Payload *payload) {
    payload->payloadLen = sizeof(Payload);
}