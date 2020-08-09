/* File: SerIODriver.h

Author: Henry Chuang

Date: 2/25/2020

Purpose: Contains SerIODriver.c function prototypes
Also contains USART_TXE and USART_RXNE bitmasks for use in IO
*/

#ifndef SerIODriver_H
#define SerIODriver_H

#include <includes.h>
#include "assert.h"
#include "BfrPair.h"

/*---- c o n s t a n t   d e f i n i t i o n s ----*/

// If not already defined, use USART_TXE as bitmask to check USART2->SR for TXE value
#ifndef USART_TXE
#define USART_TXE 0x80
#endif

// If not already defined, use USART_RXNE as bitmask to check USART2->SR for RXNE value
#ifndef USART_RXNE
#define USART_RXNE 0x20
#endif

// Timeout time for pend function for iBuffer semaphore
#define IBufferTimeout 20
#define OBufferTimeout 50
#define FlushBufferTimeout 500

#define SETENA1 (*((CPU_INT32U *) 0XE000E104))  // Address of IRQ38
#define USART2ENA 0x00000040                    // Mask to enable all interrupts

/*---- e x t e r n   d e c l a r a t i o n s ----*/

extern OS_SEM openOBfrs;                        // Number of open payload buffers
extern OS_SEM closedIBfrs;                      // Number of closed payload buffers

/*---- f u n c t i o n   d e c l a r a t i o n s ----*/

// Initialize Serial IO
CPU_VOID InitSerIO(CPU_VOID);

// Put byte into oBfrPair
CPU_INT16S PutByte(CPU_INT16S txChar);

// Output byte to Tx
CPU_VOID ServiceTx(CPU_VOID);

// Get byte from iBfrPair
CPU_INT16S GetByte(CPU_VOID);

// Input byte from Rx
CPU_VOID ServiceRx(CPU_VOID);

// Handle Tx and Rx interrupts
CPU_VOID SerialISR(CPU_VOID);

// Force close output put buffer
CPU_VOID FlushOBfrs(CPU_VOID);

// Reset Serial IO
CPU_VOID ResetSerIO(CPU_VOID);

#endif