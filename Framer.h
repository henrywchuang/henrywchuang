/* File: Framer.h

Author: Henry Chuang

Date: 4/4/2020

Purpose: Contains relevant data and function declarations for framer
*/

#ifndef FRAMER_H
#define FRAMER_H

#include "includes.h"
#include "Buffer.h"
#include "SerIODriver.h"
#include "MemMgr.h"
#include "assert.h"

#define P1Char          0x03                // preamble 1 byte value
#define P2Char          0xAF                // preamble 2 byte value
#define P3Char          0xEF                // preamble 3 byte value
#define XORPreamble     0x43                // XOR'ed value of preamble bytes

extern OS_Q framerQueue;

// Call this function to create the parser task
CPU_VOID CreateFramerTask(CPU_VOID);

// Framer task
CPU_VOID Framer(CPU_VOID *pointer);

#endif