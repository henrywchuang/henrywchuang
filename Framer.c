/* File: Framer.c

Author: Henry Chuang

Date: 4/4/2020

Purpose: Contains framer task
*/

#include "Framer.h"

//----- c o n s t a n t    d e f i n i t i o n s -----

#define FramerPrio 5 // Framer task priority

/* Size of the Process task stack */
#define	FRAMER_STK_SIZE     256 

//----- g l o b a l s -----

// Process Task Control Block
OS_TCB framerTCB;

// Stack space for Process task stack
CPU_STK	framerStk[FRAMER_STK_SIZE];

// Passes buffers from other sources to framer
OS_Q framerQueue;

/*----- C r e a t e F r a m e r T a s k ( ) -----

PURPOSE
Create and initialize the Framer Task.
*/
CPU_VOID CreateFramerTask(CPU_VOID)
{
	OS_ERR		osErr;/* -- OS Error code */
        
        OSQCreate(&framerQueue, "Framer Queue", PoolSize, &osErr);
	assert(osErr == OS_ERR_NONE);

	/* Create Framer task. */	
  OSTaskCreate(&framerTCB,
               "Framer Task",
               Framer, 
               NULL,
               FramerPrio,
               &framerStk[0],
               FRAMER_STK_SIZE / 10,
               FRAMER_STK_SIZE,
               0,
               0,
               0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &osErr);
  assert(osErr == OS_ERR_NONE);
}

/*----- F r a m e r ( ) -----

PURPOSE
Accepts all final payloads and packages them into a packet to send to output
*/
CPU_VOID Framer(CPU_VOID *pointer) {
    OS_ERR osErr;
    OS_MSG_SIZE msgSize;
    PktBfr *framerBuffer;
    Payload *payload = NULL;
    CPU_INT08U checksum = 0;           // XORs all data from packet
    
    for(;;) {
        if(payload == NULL) {
            payload = OSQPend(&framerQueue, 0, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
        
        framerBuffer = (PktBfr *) payload;
        
        checksum = XORPreamble;
        PutByte(P1Char);
        PutByte(P2Char);
        PutByte(P3Char);
        
        PutByte(framerBuffer->payloadLen+nonPayloadSize);
        checksum ^= (framerBuffer->payloadLen+nonPayloadSize);
        
        for(CPU_INT08U i = 0; i < framerBuffer->payloadLen-1; i++) {
            PutByte(framerBuffer->data[i]);
            checksum ^= framerBuffer->data[i];
        }
        
        PutByte(checksum);
        
        if(payload != NULL) {
            Free(payload);
            payload = NULL;
        }
        
        FlushOBfrs();
    }
}