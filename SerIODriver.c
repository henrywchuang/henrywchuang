/* File: SerIODriver.c

Author: Henry Chuang

Date: 2/25/2020

Purpose: Contains I/O functions to move data between buffers and USART data register
Also contains buffer size, input buffer pair, output buffer pair definitions
Program 3 revision includes an interrupt handler to handle interrupts
*/

#include "SerIODriver.h"

// Allocate the input buffer pair.
static BfrPair iBfrPair;
static CPU_INT08U iBfr0Space[IOBfrSize];
static CPU_INT08U iBfr1Space[IOBfrSize];

// Allocate the output buffer pair.
static BfrPair oBfrPair;
static CPU_INT08U oBfr0Space[IOBfrSize];
static CPU_INT08U oBfr1Space[IOBfrSize];

OS_SEM openOBfrs;                        // Number of open payload buffers
OS_SEM closedIBfrs;                      // Number of closed payload buffers

/*---------------- InitSerIO() ----------------

Purpose:
Initialize the RS232 I/O driver by:
Initializing both iBfrPair and oBfrPair.
Unmasking the Tx and the Rx and enable IRQ38.
Creating semaphores openOBfrs = 1, closedIBfrs = 0

Parameters: none

Return value: none
*/

CPU_VOID InitSerIO(CPU_VOID) {
    OS_ERR osErr;               // O/S Error code

    // Initialize I/O buffer pairs
    BfrPairInit(&iBfrPair, iBfr0Space, iBfr1Space);
    BfrPairInit(&oBfrPair, oBfr0Space, oBfr1Space);

    // Unmask Tx and Rx
    USART2->CR1 |= USART_TXE | USART_RXNE;
    
    // Enable IRQ38
    SETENA1 = USART2ENA;
    
    // Initialize openOBfrs semaphore
    OSSemCreate(&openOBfrs, "Open OBfrs", 1, &osErr);
    assert(osErr == OS_ERR_NONE);
    
    // Initialize closedIBfrs semaphore
    OSSemCreate(&closedIBfrs, "Closed IBfrs", 0, &osErr);
    assert(osErr == OS_ERR_NONE);
}

/*---------------- PutByte() ----------------

Purpose:
Write a byte to oBfrPair
If oBfrPair put buffer is closed, pend openOBfrs semaphore and swap oBfrPair.
Add byte to oBfrPair, unmask the Tx interrupt, and return added byte.

Parameters: the byte to be transmitted (CPU_INT16S txChar)

Return value: the character that was added to the put buffer, 
or -1 if put buffer was full
*/

CPU_INT16S PutByte(CPU_INT16S txChar) {
    CPU_INT16S retChar;
    OS_ERR osErr;

    if(PutBfrIsClosed(&oBfrPair)) {
        OSSemPend(&openOBfrs, OBufferTimeout, OS_OPT_PEND_BLOCKING, NULL, &osErr);
        assert(osErr == OS_ERR_NONE);

        BfrPairSwap(&oBfrPair);
    }

    retChar = PutBfrAddByte(&oBfrPair, txChar);
    
    USART2->CR1 |= USART_TXE;
    
    return retChar;
}

/*---------------- ServiceTx() ----------------

Purpose:
Output a byte from oBfrPair
If TXE = 0, return.
If TXE = 1 and oBfrPair get buffer is open, mask TX interrupt and return.
If TXE = 1 and oBfrPair get buffer is closed, output byte to Tx,
then if oBfrPair get buffer is closed, return;
otherwise, post openOBfrs semaphore and return.

Parameters: none

Return value: none
*/

CPU_VOID ServiceTx(CPU_VOID) {
    OS_ERR osErr;
    
    if(!(USART2->SR & USART_TXE)) {
    }
    else if(!GetBfrIsClosed(&oBfrPair)) {
        USART2->CR1 &= ~USART_TXE;
    }
    else {
        CPU_INT16S c = GetBfrRemByte(&oBfrPair);
        USART2->DR = c;
        
        if(!GetBfrIsClosed(&oBfrPair)) {
            OSSemPost(&openOBfrs, OS_OPT_POST_1, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
    }
}

/*---------------- GetByte() ----------------

Purpose:
Read a byte from iBfrPair
If the iBfrPair get buffer is open, pend closedIBfrs semaphore and swap iBfrPair.
Remove byte from iBfrPair, unmask the Rx interrupt, and return removed byte.

Parameters: none

Return value: the character read from get buffer, or -1 if buffer was empty
*/

CPU_INT16S GetByte(CPU_VOID) {
    CPU_INT16S retChar;
    OS_ERR osErr;

    while(!GetBfrIsClosed(&iBfrPair)) {      
        OSSemPend(&closedIBfrs, IBufferTimeout, OS_OPT_PEND_BLOCKING, NULL, &osErr);
        assert(osErr == OS_ERR_TIMEOUT || osErr == OS_ERR_NONE);
        
        if(osErr != OS_ERR_TIMEOUT || (osErr == OS_ERR_TIMEOUT && !BfrEmpty(&iBfrPair.buffers[iBfrPair.putBfrNum]))) {
            PutBfrClose(&iBfrPair);
            BfrPairSwap(&iBfrPair);
        }
    }

    retChar = GetBfrRemByte(&iBfrPair);

    USART2->CR1 |= USART_RXNE;
    
    return retChar;
}

/*---------------- ServiceRx() ----------------

Purpose:
If RXNE = 0, return.
If RXNE = 1 and iBfrPair put buffer is closed, mask Rx interrupt and return.
If RXNE = 1 and iBfrPair put buffer is open, read byte from Rx,
then if iBfrPair put buffer is open, return;
otherwise, post closedIBfrs semaphore and return.

Parameters: none

Return value: none
*/

CPU_VOID ServiceRx(CPU_VOID) {
    OS_ERR osErr;

    if(!(USART2->SR & USART_RXNE)) {
    }
    else if(PutBfrIsClosed(&iBfrPair)) {
        USART2->CR1 &= ~USART_RXNE;
    }
    else {
        CPU_INT16S c = USART2->DR;
        PutBfrAddByte(&iBfrPair, c);
        
        if(PutBfrIsClosed(&iBfrPair)) {
            OSSemPost(&closedIBfrs, OS_OPT_POST_1, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
    }
}

/*---------------- SerialISR() ----------------

Purpose:
Call ServiceRx() to handle Rx interrupts and then
call ServiceTx() to handle Tx interrupts.

Parameters: none

Return value: none
*/

CPU_VOID SerialISR(CPU_VOID) {
    ServiceRx();

    ServiceTx();
}

/*---------------- FlushOBfrs() ----------------

Purpose:
Force the put buffer to close at the end of every reply message

Parameters: none

Return value: none
*/

CPU_VOID FlushOBfrs(CPU_VOID) {
    OS_ERR osErr;
    
    if(!BfrEmpty(&oBfrPair.buffers[oBfrPair.putBfrNum])) {
        PutBfrClose(&oBfrPair);
        OSSemPend(&openOBfrs, FlushBufferTimeout, OS_OPT_PEND_BLOCKING, NULL, &osErr);
        assert(osErr == OS_ERR_NONE);
        BfrPairSwap(&oBfrPair);
        USART2->CR1 |= USART_TXE;
    }
}

/*---------------- ResetSerIO() ----------------

Purpose:
Reset serial IO by deleting semaphores,
then running InitSerIO()

Parameters: none

Return value: none
*/

CPU_VOID ResetSerIO(CPU_VOID) {
    OS_ERR osErr;
    
    OSSemDel(&openOBfrs, OS_OPT_DEL_ALWAYS, &osErr);
    assert(osErr == OS_ERR_NONE);
    
    OSSemDel(&closedIBfrs, OS_OPT_DEL_ALWAYS, &osErr);
    assert(osErr == OS_ERR_NONE);
    
    InitSerIO();
}