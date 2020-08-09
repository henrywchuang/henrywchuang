/* File: Buffer.h

Author: Henry Chuang

Date: 2/25/2020

Purpose: Contains Buffer.c function prototypes
Also contains definition of Buffer, zero, one, and failure
*/

#ifndef Buffer_H
#define Buffer_H

#include <includes.h>

#define IOBfrSize 4                     // Size of buffers used in IO
#define nonPayloadSize 4                // # of bytes in a packet that aren't data

typedef struct {
    volatile CPU_BOOLEAN closed;        // True if buffer has data ready to process
    CPU_INT08U  *putIndex;              // The position where the next byte is added
    CPU_INT08U  *getIndex;              // The position of the next byte to remove
    CPU_INT08U  *buffer;                // The address of an array of bytes serving as the buffer's data space
} Buffer;

CPU_VOID BfrInit(Buffer *bfr);
CPU_VOID BfrReset(Buffer *bfr);
CPU_BOOLEAN BfrClosed(Buffer *bfr);
CPU_VOID BfrClose(Buffer *bfr);
CPU_VOID BfrOpen(Buffer *bfr);
CPU_BOOLEAN BfrFull(Buffer *bfr);
CPU_BOOLEAN BfrEmpty(Buffer *bfr);
CPU_INT08S BfrAddByte(Buffer *bfr, CPU_INT08S theByte);
CPU_INT08S BfrNextByte(Buffer *bfr);
CPU_INT08S BfrRemByte(Buffer *bfr);

typedef struct {
    CPU_INT08S xPos;
    CPU_INT08S yPos;
} Position;

typedef struct {
    CPU_INT08U payloadLen;              // Length of payload
    CPU_INT08U data[1];                 // Payload data bytes
} PktBfr;

typedef struct {
    CPU_INT08U payloadLen;              // Length of payload
    CPU_INT08U dstAddr;                 // Destination address
    CPU_INT08U srcAddr;                 // Source address
    CPU_INT08U pktType;                 // Message type
    union {                             // Data part of payload
        struct {                        // Move, path, loop command
            CPU_INT08U  robotAddr;      // Address of robot to be moved
            Position    pos[10];        // Up to 10 stops
        } mgrCmd;
    CPU_INT08U          direction;      // Step command direction
    Position            position;       // Here I am command
    CPU_INT08U          cmdType;        // ACK command type
    CPU_INT08U          errCode;        // Error code reply
    } cmdExt;
} Payload;

#endif