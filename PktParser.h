/* File: PktParser.h

Author: Henry Chuang

Date: 4/4/2020

Purpose: Contains relevant data and function declarations for packet parser
*/

#ifndef PKTPARSER_H
#define PKTPARSER_H

#include "includes.h"
#include "Buffer.h"
#include "SerIODriver.h"
#include "MemMgr.h"
#include "assert.h"
#include "Framer.h"

#define minPacketLength 8               // minimum packet length
#define maxPacketLength 29              // maximum packet length
#define P1Char 0x03                     // preamble 1 byte value
#define P2Char 0xAF                     // preamble 2 byte value
#define P3Char 0xEF                     // preamble 3 byte value
#define P1Error 1                       // preamble 1 byte error return value
#define P2Error 2                       // preamble 2 byte error return value
#define P3Error 3                       // preamble 3 byte error return value
#define ChecksumError 4                 // checksum error return value
#define SizeError 5                     // packet size error return value
#define ReplyPacketSize 5               // size of error packet
#define CCAddress 1                     // control center address
#define RMAddress 2                     // robot manager address
#define ErrorMessageType 0x0B           // error message type for packet

//----- e n u m e r a t i o n -----

typedef enum {P1, P2, P3, L, D, S, ER} ParserState;

//----- e x t e r n    d e c l a r a t i o n s -----

// Passes buffers from parser to robot manager
extern OS_Q parserQueue;

//----- f u n c t i o n    p r o t o t y p e s -----

// Call this function to create the parser task
CPU_VOID CreateParserTask(CPU_VOID);

// Parser task
CPU_VOID ParsePkt(CPU_VOID *pointer);

// Write packet data into payload buffer
CPU_VOID InsertPacketData(PktBfr **pktBfr, CPU_BOOLEAN *errorBit);

// Send error packet to framer queue
CPU_VOID ParserError(PktBfr **pktBfr, CPU_INT08U errorByte);

// Parser action in P1 state
ParserState ParserP1Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **pktBfr);

// Parser action in P2 state
ParserState ParserP2Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **pktBfr);

// Parser action in P3 state
ParserState ParserP3Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **pktBfr);

// Parser action in L state
ParserState ParserLengthAction(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **pktBfr);

// Parser action in D state
ParserState ParserDataAction(CPU_INT08U byte, CPU_INT08U *count, PktBfr **pktBfr);

// Parser action in S state
ParserState ParserChecksumAction(CPU_INT08U checksum, CPU_BOOLEAN *errorBit, PktBfr **pktBfr);

// Parser action in ER state
ParserState ParserErrorAction(CPU_INT08U byte, CPU_INT08U *checksum);

#endif