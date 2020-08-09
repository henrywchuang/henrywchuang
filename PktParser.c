/* File: PktParser.c

Author: Henry Chuang

Date: 4/4/2020

Purpose: Contains packet parser task
*/

#include "PktParser.h"

//----- c o n s t a n t    d e f i n i t i o n s -----

#define ParserPrio 6 // Parser task priority

/* Size of the Process task stack */
#define	PARSER_STK_SIZE     256 

//----- g l o b a l s -----

// Process Task Control Block
OS_TCB parserTCB;

// Stack space for Process task stack
CPU_STK			parserStk[PARSER_STK_SIZE];

// Passes buffers from parser to robot manager
OS_Q parserQueue;

/*----- C r e a t e P a r s e r T a s k ( ) -----

PURPOSE
Create and initialize the Parser Task.
*/
CPU_VOID CreateParserTask(CPU_VOID)
{
	OS_ERR		osErr;/* -- OS Error code */
        
        OSQCreate(&parserQueue, "Parser Queue", PoolSize, &osErr);
	assert(osErr == OS_ERR_NONE);

	/* Create Parser task. */	
  OSTaskCreate(&parserTCB,
               "Packet Parser Task",
               ParsePkt, 
               NULL,
               ParserPrio,
               &parserStk[0],
               PARSER_STK_SIZE / 10,
               PARSER_STK_SIZE,
               0,
               0,
               0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &osErr);
  assert(osErr == OS_ERR_NONE);
}

/*----- P a r s e P k t ( ) -----

PURPOSE
Parser task, reads incoming bytes and either generates a payload or a one-byte error payload
Places payload into payload buffer
*/
CPU_VOID ParsePkt(CPU_VOID *pointer) {
    ParserState parseState = P1;        // Indicates state of parser
    CPU_INT08U  inByte;                 // 1-byte representation of data stream read
    CPU_INT08U  payloadCount = 0;       // Increments until payload size is reached
    CPU_BOOLEAN errBit = 0;             // errorBit = 1 prevents parser from sending multiple errors
    CPU_INT08U  checksum = 0;           // XORs all data from packet
    PktBfr      *pktBfr = NULL;         // Extract payload from packet
    
    for(;;) {
        if(pktBfr == NULL)
            pktBfr = (PktBfr *) Allocate();
        
        inByte = GetByte();                                                                     // Get byte from iBuffer
        checksum ^= inByte;                                                                     // XOR byte to checksum
        switch(parseState) {                                                                    // Check parser state
            case P1:                                                                            // If parser is in P1 state
                parseState = ParserP1Action(inByte, &errBit, &pktBfr);                      // Do parser P1 action
                break;
            case P2:                                                                            // " " " P2 "
                parseState = ParserP2Action(inByte, &errBit, &pktBfr);                      // " " P2 "
                break;
            case P3:                                                                            // " " " P3 "
                parseState = ParserP3Action(inByte, &errBit, &pktBfr);                      // " " P3 "
                break;
            case L:                                                                             // " " " L "
                parseState = ParserLengthAction(inByte, &errBit, &pktBfr);                  // " " L "
                break;                                                                          
            case D:                                                                             // " " " D "
                parseState = ParserDataAction(inByte, &payloadCount, &pktBfr);                // " " D "
                break;
            case S:                                                                             // " " " S "
                parseState = ParserChecksumAction(checksum, &errBit, &pktBfr);              // " " S "
                break;
            case ER:                                                                            // " " " ER "
                parseState = ParserErrorAction(inByte, &checksum);                              // " " ER "
                break;
        }
    }
}

/*---------------- InsertPacketData() ----------------

Purpose:
Inserts data from packet to payload buffer,
resets error bit

Parameters: PktBfr *payload, CPU_BOOLEAN *errorBit

Return value: none
*/

CPU_VOID InsertPacketData(PktBfr **parserBfr, CPU_BOOLEAN *errorBit) {
    OS_ERR osErr;
    Payload **payload = (Payload **) parserBfr;

    OSQPost(&parserQueue, *payload, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    *parserBfr = NULL;
    
    *errorBit = 0;                                                      // Reset errorBit
}

/*---------------- ParserError() ----------------

Purpose:
Sends error packet directly to framer

Parameters: CPU_INT08U errorByte

Return value: none
*/

CPU_VOID ParserError(PktBfr **parserBfr, CPU_INT08U errorByte) {
    OS_ERR osErr;
    Payload **payload;
    
    CPU_INT08U ErrorPacket[ReplyPacketSize-1] = {CCAddress, RMAddress, ErrorMessageType, errorByte};
    
    (*parserBfr)->payloadLen = ReplyPacketSize;
    
    for(CPU_INT08U i = 0; i < ReplyPacketSize-1; i++)
        (*parserBfr)->data[i] = ErrorPacket[i];
    
    payload = (Payload **) parserBfr;
    
    OSQPost(&framerQueue, *payload, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    *parserBfr = NULL;
}

/*---------------- ParserP1Action() ----------------

Purpose:
If parser detects P1 byte, parser moves to P2 state
Otherwise, if error bit is not set, parser creates P1 error payload and sets error bit
before going to ER state
If error bit is set, parser simply goes to ER state

Parameters: CPU_INT08U byte, CPU_BOOLEAN *errorBit

Return value: ParserState
*/

ParserState ParserP1Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **parserBfr) {
    if(byte != P1Char) {
        if(!(*errorBit)) {
            ParserError(parserBfr, P1Error);
            
            *errorBit = 1;
        }
        return ER;
    }
    else
        return P2;
}

/*---------------- ParserP2Action() ----------------

Purpose:
If parser detects P2 byte, parser moves to P3 state
Otherwise, if error bit is not set, parser creates P2 error payload and sets error bit
before going to ER state
If error bit is set, parser simply goes to ER state

Parameters: CPU_INT08U byte, CPU_BOOLEAN *errorBit

Return value: ParserState
*/

ParserState ParserP2Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **parserBfr) {
    if(byte != P2Char) {
        if(!(*errorBit)) {
            ParserError(parserBfr, P2Error);
            
            *errorBit = 1;
        }
        return ER;
    }
    else
        return P3;
}

/*---------------- ParserP3Action() ----------------

Purpose:
If parser detects P3 byte, parser moves to L state
Otherwise, if error bit is not set, parser creates P3 error payload and sets error bit
before going to ER state
If error bit is set, parser simply goes to ER state

Parameters: CPU_INT08U byte, CPU_BOOLEAN *errorBit

Return value: ParserState
*/

ParserState ParserP3Action(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **parserBfr) {
    if(byte != P3Char) {
        if(!(*errorBit)) {
            ParserError(parserBfr, P3Error);

            *errorBit = 1;
        }
        return ER;
    }
    else
        return L;
}

/*---------------- ParserLengthAction() ----------------

Purpose:
If parser detects length byte is an acceptable length, parser adds byte to parserBfr
Otherwise, if error bit is not set, parser creates Size error payload and sets error bit
before going to ER state
If error bit is set, parser simply goes to ER state

Parameters: CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr *parserBfr

Return value: ParserState
*/

ParserState ParserLengthAction(CPU_INT08U byte, CPU_BOOLEAN *errorBit, PktBfr **parserBfr) {
    if(byte < minPacketLength || byte > maxPacketLength) {
        if(!(*errorBit)) {
            ParserError(parserBfr, SizeError);
            
            *errorBit = 1;
        }
        return ER;
    }
    else {
        (*parserBfr)->payloadLen = byte - nonPayloadSize;
      
        return D;
    }
}

/*---------------- ParserDataAction() ----------------

Purpose:
Parser first adds byte to parserBfr data section
If the number of data bytes added has reached the payload size,
the payload counter resets and parser goes to S state
Otherwise, parser remains in D state

Parameters: CPU_INT08U byte, CPU_INT08U *payloadCount, PktBfr *parserBfr

Return value: ParserState
*/

ParserState ParserDataAction(CPU_INT08U byte, CPU_INT08U *count, PktBfr **parserBfr) {
    (*parserBfr)->data[(*count)++] = byte;
    if(*count >= (*parserBfr)->payloadLen-1) {
        *count = 0;
        return S;
    }
    else
        return D;
}

/*---------------- ParserChecksumAction() ----------------

Purpose:
If parser detects detects that checksum is equal to 0,
parser inserts data from parserBfr to payloadBfr and brings parser back to P1 state
Otherwise, if error bit is not set, parser creates Checksum error payload and sets error bit
before going to ER state
If error bit is set, parser simply goes to ER state

Parameters: CPU_INT08U checksum, CPU_BOOLEAN *errorBit, PktBfr *parserBfr

Return value: ParserState
*/

ParserState ParserChecksumAction(CPU_INT08U checksum, CPU_BOOLEAN *errorBit, PktBfr **parserBfr) {
    if(checksum == 0) {
        InsertPacketData(parserBfr, errorBit);
        
        return P1;
    }
    else {
        if(!(*errorBit)) {
            ParserError(parserBfr, ChecksumError);
          
            *errorBit = 1;
        }
        return ER;
    }
}

/*---------------- ParserErrorAction() ----------------

Purpose:
If parser detects P1 byte, parser moves to P2 state and sets checksum to received byte
Otherwise, parser stays in ER state

Parameters: CPU_INT08U byte, CPU_BOOLEAN *checksum

Return value: ParserState
*/

ParserState ParserErrorAction(CPU_INT08U byte, CPU_INT08U *checksum) {
    if(byte == P1Char) {        // If preamble 1 is read, go to preamble 2 state
        *checksum = byte;       // Reset checksum
        return P2;
    }
    else
        return ER;
}