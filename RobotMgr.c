/* File: RobotMgr.c

Author: Henry Chuang

Date: 4/18/2020

Purpose: Contains robot manager task and relevant functions
*/

#include "RobotMgr.h"

//----- c o n s t a n t    d e f i n i t i o n s -----

#define MgrPrio 4 // Robot Manager task priority

/* Size of the Process task stack */
#define	MGR_STK_SIZE     256 

#define BadMessageTypeError 61

//----- g l o b a l s -----

// Process Task Control Block
OS_TCB mgrTCB;

// Stack space for Process task stack
CPU_STK	mgrStk[MGR_STK_SIZE];

// Timer
OS_TMR mgrTimer;

/*----- C r e a t e R o b o t M g r T a s k ( ) -----

PURPOSE
Create and initialize the Robot Manager Task.
*/
CPU_VOID CreateRobotMgrTask(CPU_VOID) {
    	OS_ERR		osErr;/* -- OS Error code */

	/* Create Manager task. */	
  OSTaskCreate(&mgrTCB,
               "Manager Task",
               RobotMgr, 
               NULL,
               MgrPrio,
               &mgrStk[0],
               MGR_STK_SIZE / 10,
               MGR_STK_SIZE,
               0,
               0,
               0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &osErr);
  assert(osErr == OS_ERR_NONE);
  
  for(CPU_INT08U i = 0; i < MaxRobots; i++) {
      roboStats[i].currentPos.xPos = -1;
      roboStats[i].currentPos.yPos = -1;
  }
  
  OSSemCreate(&statusAccess, "Status access", 1, &osErr);
  assert(osErr == OS_ERR_NONE);
  
  OSTmrCreate(&mgrTimer, "RobotMgr Timer", 20, 0, OS_OPT_TMR_ONE_SHOT, NULL, NULL, &osErr);
}

/*----- R o b o t M g r ( ) -----

PURPOSE
Robot Manager task, takes in instructions from parser and forwards instructions
to appropriate robot controller.  Also, can create robots with AddRobot instruction
Forwards an ACK packet to Framer task for every successful instruction and an error
packet for every instruction that has a bad message type
*/
CPU_VOID RobotMgr(CPU_VOID *pointer) {
    OS_ERR osErr;
    OS_MSG_SIZE msgSize;
    CPU_INT08U replyCode;                       // replyCode:
    Payload *mgrIOBfr = NULL;                    // 0 for don't send anything
    Payload *mgrReplyBfr = NULL;                 // 1 for add robot
                                                // 61 for bad message type
    for(;;) {                                   // 11, 12, 13, 14, 21, 22, 23, 31, 32, 33, 41, 42, 43, 51, 52 for all other error messages
        replyCode = 0;
        
        if(mgrIOBfr == NULL) {
            mgrIOBfr = OSQPend(&parserQueue, 0, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
        
        if(mgrIOBfr->pktType == 0)
            ResetMgr();
        else if(mgrIOBfr->pktType == 1) {
            OSSemPend(&statusAccess, 0, OS_OPT_PEND_BLOCKING, NULL, &osErr);
            assert(osErr == OS_ERR_NONE);
            
            replyCode = AddRobot(mgrIOBfr);
            
            OSSemPost(&statusAccess, OS_OPT_POST_1, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
        else if(mgrIOBfr->pktType >= 2 && mgrIOBfr->pktType <= 5)
            replyCode = ForwardCommand(mgrIOBfr);
        else if(mgrIOBfr->pktType == 9)
            ForwardHereIAm(mgrIOBfr);
        else
            replyCode = BadMessageTypeError;
        
        OSTimeDly(5, OS_OPT_TIME_DLY, &osErr);                     // Prevents overloading of the Tx
        assert(osErr == OS_ERR_NONE);
        
        ReplyHandler(mgrReplyBfr, replyCode);
        
        if(replyCode == 1)
            Free(mgrIOBfr);
        
        mgrIOBfr = NULL;
    }
}

/*---------------- ReplyHandler() ----------------

Purpose:
Creates packet to send to control center
If replyVal = 0, send nothing because command was reset
If replyVal > 0, < 10, send ACK packet
If replyVal > 10, send error packet

Parameters: PktBfr **mgrReplyBfr, CPU_INT08U replyVal

Return value: none
*/

CPU_VOID ReplyHandler(Payload *mgrReplyBfr, CPU_INT08U replyVal) {
    OS_ERR osErr;
  
    if(replyVal == 0 || replyVal == 9)
        return;

    mgrReplyBfr = Allocate();
  
    mgrReplyBfr->payloadLen = ReplyPacketSize;
    mgrReplyBfr->dstAddr = CCAddress;
    mgrReplyBfr->srcAddr = RMAddress;

    if(replyVal >= 1 && replyVal <= 5) {
        mgrReplyBfr->pktType = ACKMessageType;
        mgrReplyBfr->cmdExt.cmdType = replyVal;
    }
    else {
        mgrReplyBfr->pktType = ErrorMessageType;
        mgrReplyBfr->cmdExt.errCode = replyVal;
    }
    
    OSQPost(&framerQueue, mgrReplyBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    mgrReplyBfr = NULL;
}

/*---------------- CommandHandler() ----------------

Purpose:
Creates packet to send to robot controller

Parameters: PktBfr *mgrIOBfr

Return value: none
*/

CPU_VOID CommandHandler(Payload *mgrIOBfr) {
    OS_ERR osErr;
    
    OSQPost(&framerQueue, mgrIOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    mgrIOBfr = NULL;
}

/*---------------- BadRobotAddress() ----------------

Purpose:
Checks if given robot address is allowed
Only addresses between 3 and 15 are allowed

Parameters: PktBfr *mgrIOBfr

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN BadRobotAddress(Payload *mgrIOBfr) {
    return (mgrIOBfr->cmdExt.mgrCmd.robotAddr > 15 || mgrIOBfr->cmdExt.mgrCmd.robotAddr < 3);
}

/*---------------- NonExistentRobot() ----------------

Purpose:
Checks if a robot has been created
A robot has been created if its address is set in roboStats

Parameters: PktBfr *mgrIOBfr

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN NonExistentRobot(Payload *mgrIOBfr) {
    for(CPU_INT08U i = 0; i < MaxRobots; i++)
        if(roboStats[i].address == mgrIOBfr->cmdExt.mgrCmd.robotAddr)
            return 0;
    
    return 1;
}

/*---------------- BadLocation() ----------------

Purpose:
Checks if a location is bad
A location is bad if its x coordinate is > 39 or if its y coordinate is > 18

Parameters: PktBfr *mgrIOBfr

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN BadLocation(Payload *mgrIOBfr) {
    for(CPU_INT08U i = 0; i < (mgrIOBfr->payloadLen-5)/2; i++)
        if(mgrIOBfr->cmdExt.mgrCmd.pos[i].xPos > 39 || mgrIOBfr->cmdExt.mgrCmd.pos[i].yPos > 18)
            return 1;
    
    return 0;
}

/*---------------- LocationOccupied() ----------------

Purpose:
Checks if a location is occupied
A location is occupied if there is another robot that is in its target position

Parameters: PktBfr *mgrIOBfr

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN LocationOccupied(Payload *mgrIOBfr) {
    return CheckLocation(GenPosCoord(mgrIOBfr->cmdExt.mgrCmd.pos[0].xPos, mgrIOBfr->cmdExt.mgrCmd.pos[0].yPos));
}
