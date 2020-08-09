/* File: BfrPair.h

Author: Henry Chuang

Date: 2/7/2020

Purpose: Contains BfrPair.c function prototypes
Also contains definition of BfrPair and NumBfrs
*/

#ifndef BfrPair_H
#define BfrPair_H

#include <includes.h>
#include "Buffer.h"
#include "MemMgr.h"

// Define number of buffers per buffer pair
#define NumBfrs 2

typedef struct {
    CPU_INT08U  putBfrNum;              // The index of the put buffer (either 0 or 1)
    Buffer      buffers[NumBfrs];       // The buffer pair's 2 buffers
} BfrPair;

CPU_VOID BfrPairInit(BfrPair *bfrPair, CPU_INT08U *bfr0Space, CPU_INT08U *bfr1Space);
CPU_VOID PutBfrReset(BfrPair *bfrPair);
CPU_INT08U *PutBfrDataSpace(BfrPair *bfrPair);
CPU_INT08U *GetBfrDataSpace(BfrPair *bfrPair);
CPU_BOOLEAN PutBfrIsClosed(BfrPair *bfrPair);
CPU_BOOLEAN GetBfrIsClosed(BfrPair *bfrPair);
CPU_VOID PutBfrClose(BfrPair *bfrPair);
CPU_VOID GetBfrOpen(BfrPair *bfrPair);
CPU_INT16S PutBfrAddByte(BfrPair *bfrPair, CPU_INT16S byte);
CPU_INT16S GetBfrNextByte(BfrPair *bfrPair);
CPU_INT16S GetBfrRemByte(BfrPair *bfrPair);
CPU_BOOLEAN BfrPairSwappable(BfrPair *bfrPair);
CPU_VOID BfrPairSwap(BfrPair *bfrPair);
CPU_BOOLEAN BfrPairEmpty(BfrPair *bfrPair);

#endif