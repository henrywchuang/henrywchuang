/* File: BfrPair.c

Author: Henry Chuang

Date: 2/7/2020

Purpose: Contains functions manipulating buffer pairs.
Each pair has a put buffer, which takes in data, and a get buffer, which has data taken from it.
The functions include buffer pair initialization, resetting buffers, obtaining addresses of buffers,
testing whether buffer is open or closed, adding or removing bytes, checking for swapping,
and swapping put buffer and get buffer.
*/

#include "BfrPair.h"

/*---------------- BfrPairInit() ----------------

Purpose:
Initialize both buffers of the buffer pair.

Parameters: buffer pair address
            address of buffer 0 data space
            address of buffer 1 data space
            buffer capacity in bytes

Return value: none
*/

CPU_VOID BfrPairInit(BfrPair *bfrPair, CPU_INT08U *bfr0Space, CPU_INT08U *bfr1Space) {
    bfrPair->buffers[0].buffer = bfr0Space;
    bfrPair->buffers[1].buffer = bfr1Space;
    BfrInit(&bfrPair->buffers[0]);
    BfrInit(&bfrPair->buffers[1]);
    bfrPair->putBfrNum = 0;
}

/*---------------- PutBfrReset() ----------------

Purpose:
Reset the put buffer.

Parameters: buffer pair address

Return value: none
*/

CPU_VOID PutBfrReset(BfrPair *bfrPair) {
    BfrReset(&bfrPair->buffers[bfrPair->putBfrNum]);
}

/*---------------- PutBfrDataSpace() ----------------

Purpose:
Obtain the address of the put buffer's buffer data space.

Parameters: buffer pair address

Return value: the address of the put buffer's data space
*/

CPU_INT08U *PutBfrDataSpace(BfrPair *bfrPair) {
    return bfrPair->buffers[bfrPair->putBfrNum].buffer;
}

/*---------------- GetBfrDataSpace() ----------------

Purpose:
Obtain the address of the get buffer's buffer data space.

Parameters: buffer pair address

Return value: the address of the get buffer's data space
*/

CPU_INT08U *GetBfrDataSpace(BfrPair *bfrPair) {
    return bfrPair->buffers[1 - bfrPair->putBfrNum].buffer;
}

/*---------------- PutBfrIsClosed() ----------------

Purpose:
Test whether or not the put buffer is closed.

Parameters: buffer pair address

Return value: TRUE if the put buffer is closed, otherwise FALSE
*/

CPU_BOOLEAN PutBfrIsClosed(BfrPair *bfrPair) {
    return BfrClosed(&bfrPair->buffers[bfrPair->putBfrNum]);
}

/*---------------- GetBfrIsClosed() ----------------

Purpose:
Test whether or not the get buffer is closed.

Parameters: buffer pair address

Return value: TRUE if the get buffer is closed, otherwise FALSE
*/

CPU_BOOLEAN GetBfrIsClosed(BfrPair *bfrPair) {
    return BfrClosed(&bfrPair->buffers[1 - bfrPair->putBfrNum]);
}

/*---------------- PutBfrClose() ----------------

Purpose:
Mark the put buffer closed.

Parameters: buffer pair address

Return value: none
*/

CPU_VOID PutBfrClose(BfrPair *bfrPair) {
    BfrClose(&bfrPair->buffers[bfrPair->putBfrNum]);
}

/*---------------- GetBfrOpen() ----------------

Purpose:
Mark the get buffer open.

Parameters: buffer pair address

Return value: none
*/

CPU_VOID GetBfrOpen(BfrPair *bfrPair) {
    BfrOpen(&bfrPair->buffers[1 - bfrPair->putBfrNum]);
}

/*---------------- PutBfrAddByte() ----------------

Purpose:
Add a byte to the put buffer at position "putIndex" and increment "putIndex" by 1.
If the buffer becomes full, mark it closed.
If the buffer is already full, return -1.

Parameters: buffer pair address

Return value: the byte added, unless the buffer was full; otherwise -1
*/

CPU_INT16S PutBfrAddByte(BfrPair *bfrPair, CPU_INT16S byte) {
    return BfrAddByte(&bfrPair->buffers[bfrPair->putBfrNum], byte);
}

/*---------------- GetBfrNextByte() ----------------

Purpose:
Return the byte from position "getIndex" of the get buffer,
Or return -1 if the get buffer is empty.

Parameters: buffer pair address

Return value: the byte from position "getIndex" of the get buffer unless get buffer is empty;
otherwise -1

Note: unused
*/

CPU_INT16S GetBfrNextByte(BfrPair *bfrPair) {
    return BfrNextByte(&bfrPair->buffers[1 - bfrPair->putBfrNum]);
}

/*---------------- GetBfrRemByte() ----------------

Purpose:
Return the byte from position "getIndex" in the get buffer and increment "getIndex by 1.
If the buffer becomes empty, mark it open.
If the buffer is already empty, return -1.

Parameters: buffer pair address

Return value: the byte from position "getIndex" of the get buffer unless get buffer is empty;
otherwise -1
*/

CPU_INT16S GetBfrRemByte(BfrPair *bfrPair) {
    return BfrRemByte(&bfrPair->buffers[1 - bfrPair->putBfrNum]);
}

/*---------------- BfrPairSwappable() ----------------

Purpose:
Test wether or not a buffer pair is ready to be swapped.
It is ready if the put buffer is closed and the get buffer is open.

Parameters: buffer pair address

Return value: TRUE if ready to swap; otherwise, FALSE
*/

CPU_BOOLEAN BfrPairSwappable(BfrPair *bfrPair) {
    return PutBfrIsClosed(bfrPair) && !GetBfrIsClosed(bfrPair);
}

/*---------------- BfrPairSwap() ----------------

Purpose:
Swap the put buffer and the get buffer,
and reset the put buffer.

Parameters: buffer pair address

Return value: none
*/

CPU_VOID BfrPairSwap(BfrPair *bfrPair) {
    bfrPair->putBfrNum = 1 - bfrPair->putBfrNum;
    
    PutBfrReset(bfrPair);
}

/*---------------- BfrPairEmpty() ----------------

Purpose:
Return true if both put buffer and get buffer are empty,
else return false

Parameters: buffer pair address

Return value: TRUE if both buffers empty, else FALSE
*/

CPU_BOOLEAN BfrPairEmpty(BfrPair *bfrPair) {
    return (BfrEmpty(&bfrPair->buffers[bfrPair->putBfrNum]) && BfrEmpty(&bfrPair->buffers[1 - bfrPair->putBfrNum]));
}
