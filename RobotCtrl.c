/* File: RobotCtrl.c

Author: Henry Chuang

Date: 4/22/2020

Purpose: Contains robot control task and other relevant functions
*/

#include "RobotCtrl.h"

//----- c o n s t a n t    d e f i n i t i o n s -----

#define CtrlPrio 3 // Robot Control task priority

//----- g l o b a l s -----

// Robot Control Task Control Block
OS_TCB ctrlTCB[MaxRobots];

// Robot Control Status
RobotStatus roboStats[MaxRobots];

// Restricts access to roboStats
OS_SEM statusAccess;

/*----- C r e a t e R o b o t C t r l T a s k ( ) -----

PURPOSE
Create and initialize a Robot Control task.
*/
CPU_VOID CreateRobotCtrlTask(Payload *buffer) {
    	OS_ERR		osErr;/* -- OS Error code */
        CPU_CHAR taskName[20];
        CPU_CHAR queueName[20];
        CPU_CHAR mBoxName[20];
        sprintf(taskName, "Control Task %d", buffer->cmdExt.mgrCmd.robotAddr);
        sprintf(queueName, "Robot Queue %d", buffer->cmdExt.mgrCmd.robotAddr);
        sprintf(mBoxName, "Robot MBox %d", buffer->cmdExt.mgrCmd.robotAddr);
        
        roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].address = buffer->cmdExt.mgrCmd.robotAddr;
        
        roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].lost = FALSE;
        
        roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].dead = FALSE;
        
        roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].currentPos = buffer->cmdExt.mgrCmd.pos[0];
        
        OSQCreate(&(roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].myQueue), queueName, PoolSize, &osErr);
	assert(osErr == OS_ERR_NONE);
        
        OSQCreate(&(roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].myMBox), mBoxName, 1, &osErr);
        assert(osErr == OS_ERR_NONE);

	/* Create Controller task. */	
  OSTaskCreate(&ctrlTCB[buffer->cmdExt.mgrCmd.robotAddr-3],
               taskName,
               RobotCtrl, 
               &(roboStats[buffer->cmdExt.mgrCmd.robotAddr-3]),
               CtrlPrio,
               &(roboStats[buffer->cmdExt.mgrCmd.robotAddr-3].ctrlStk)[0],
               CTRL_STK_SIZE / 10,
               CTRL_STK_SIZE,
               0,
               10,
               0,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &osErr);
  assert(osErr == OS_ERR_NONE);
}

/*----- R o b o t C t r l ( ) -----

PURPOSE
Robot Control task, takes in commands from Robot Manager and translates them
into appropriate steps to send back to robot.  Also accepts Here I Am messages to
ensure location of robot.
*/
CPU_VOID RobotCtrl(CPU_VOID *pointer) {
    OS_ERR osErr;
    OS_MSG_SIZE msgSize;
    Payload *ctrlIBfr = NULL;
    Payload *ctrlOBfr = NULL;
    Payload *ctrlStopBfr = NULL;
    CPU_BOOLEAN stopped = FALSE;
    CPU_INT08U action;
    CPU_INT08U numPointsOnJourney;
    Position pointsOnJourney[10];
    CPU_INT08U nextDestination;
    CPU_INT08U detourCount = 0;
    
    for(;;) {
        if(roboStats[ctrlIBfr->srcAddr-3].dead == TRUE)
          break;
      
        if(ctrlIBfr == NULL) { /*----- Wait for Move Command -----*/
            ctrlIBfr = OSQPend(&((RobotStatus*) pointer)->myQueue, 0, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
            assert(osErr == OS_ERR_NONE);
        }
        
        nextDestination = 0;                                                            // Store all destinations
        action = ctrlIBfr->pktType;
        numPointsOnJourney = (ctrlIBfr->payloadLen-5)/2;
        for(CPU_INT08U i = 0; i < numPointsOnJourney; i++)
            pointsOnJourney[i] = ctrlIBfr->cmdExt.mgrCmd.pos[i];
        
        ((RobotStatus*) pointer)->destination = pointsOnJourney[nextDestination];
        
        Free(ctrlIBfr);
        ctrlIBfr = NULL;
        
        if(action == 0x05)
            stopped = TRUE;
        
        while(!stopped) {       /*----- While destination not reached -----*/
            OSSemPend(&statusAccess, 0, OS_OPT_PEND_BLOCKING, NULL, &osErr);            // Critical section start
            assert(osErr == OS_ERR_NONE);
          
            MoveRobot(((RobotStatus*) pointer)->address-3, ctrlOBfr, &detourCount);           /*----- Issue Step Command -----*/
            
            /*----- Wait for Here I Am reply -----*/
            ctrlIBfr = OSQPend(&((RobotStatus*) pointer)->myMBox, HereIAmTimeout, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
            
            if(osErr == OS_ERR_TIMEOUT) {
                LostRobot(ctrlOBfr, ((RobotStatus*) pointer)->address-3);       /*----- Deal with lost robot -----*/
                ctrlIBfr = OSQPend(&((RobotStatus*) pointer)->myMBox, HereIAmTimeout, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
                if(osErr == OS_ERR_TIMEOUT) {                                   /*----- Deal with dead robot -----*/
                    roboStats[ctrlIBfr->srcAddr-3].dead = TRUE;
                    stopped = TRUE;
                    break;
                }
            }
            else
                assert(osErr == OS_ERR_NONE);
            
            OSSemPost(&statusAccess, OS_OPT_POST_1, &osErr);                            // Critical section end
            assert(osErr == OS_ERR_NONE);
            
            if(detourCount == 10) {             // If robot is forced to go off course ten times in a row, send error message
                GiveUp(ctrlOBfr, ctrlIBfr->srcAddr);
                stopped = TRUE;
                Free(ctrlIBfr);
                ctrlIBfr = NULL;
                continue;
            }
            
            roboStats[ctrlIBfr->srcAddr-3].currentPos = ctrlIBfr->cmdExt.position;
            roboStats[ctrlIBfr->srcAddr-3].lost = FALSE;
            
            switch(action) {
                case 2:         // Move command: Stops if robot reaches destination
                    stopped = EqualPosition(roboStats[ctrlIBfr->srcAddr-3].currentPos, roboStats[ctrlIBfr->srcAddr-3].destination);
                    break;
                case 3:         // Path command: Changes destination until reaching final destination, then stops
                    if(nextDestination == numPointsOnJourney-1)
                        stopped = EqualPosition(roboStats[ctrlIBfr->srcAddr-3].currentPos, roboStats[ctrlIBfr->srcAddr-3].destination);
                    else
                        if(EqualPosition(roboStats[ctrlIBfr->srcAddr-3].currentPos, roboStats[ctrlIBfr->srcAddr-3].destination))
                            ((RobotStatus*) pointer)->destination = pointsOnJourney[++nextDestination];                        
                    break;
                case 4:         // Loop command: Cycles through destinations until acquiring stop command
                    if(EqualPosition(roboStats[ctrlIBfr->srcAddr-3].currentPos, roboStats[ctrlIBfr->srcAddr-3].destination)) {
                        if(++nextDestination == numPointsOnJourney)
                            nextDestination = 0;                        
                        ((RobotStatus*) pointer)->destination = pointsOnJourney[nextDestination];
                    }
                    ctrlStopBfr = OSQPend(&((RobotStatus*) pointer)->myQueue, StopTimeout, OS_OPT_PEND_BLOCKING, &msgSize, NULL, &osErr);
                    assert(osErr == OS_ERR_TIMEOUT || osErr == OS_ERR_NONE);
                    if(osErr == OS_ERR_NONE) {
                        if(ctrlStopBfr->pktType == 0x05)
                            stopped = TRUE;
                        Free(ctrlStopBfr);
                        ctrlStopBfr = NULL;
                    }
                    break;
            }
            
            Free(ctrlIBfr);
            ctrlIBfr = NULL;
        }
        stopped = FALSE;
    }
}

/*---------------- EqualPosition() ----------------

Purpose:
Checks whether two positions are equal

Parameters: Position a, Position b

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN EqualPosition(Position a, Position b) {
    return (a.xPos == b.xPos && a.yPos == b.yPos);
}

/*---------------- CheckLocation() ----------------

Purpose:
Checks if a location is occupied
A location is occupied if there is another robot that is in its target position

Parameters: Position pos

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN CheckLocation(Position pos) {
    CPU_BOOLEAN retVal = FALSE;
    
    for(CPU_INT08U i = 0; i < MaxRobots; i++)
        if(EqualPosition(pos, roboStats[i].currentPos))
            retVal = TRUE;
    
    return retVal;
}

/*---------------- CheckNextSpot() ----------------

Purpose:
Checks if a robot's next step location is occupied

Parameters: CPU_INT08U addr, CPU_INT08U step

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN CheckNextSpot(CPU_INT08U addr, CPU_INT08U step) {
    return CheckLocation(GenPosDir(addr, step));
}

/*---------------- GenPosCoord() ----------------

Purpose:
Creates a position based on xPos and yPos

Parameters: CPU_INT08S xPos, CPU_INT08S yPos

Return value: Position
*/

Position GenPosCoord(CPU_INT08S xPos, CPU_INT08S yPos) {
    Position generated;
    generated.xPos = xPos;
    generated.yPos = yPos;
    return generated;
}

/*---------------- GenPosCoord() ----------------

Purpose:
Creates a position based on robot address and direction

Parameters: CPU_INT08U addr, CPU_INT08U direction

Return value: Position
*/


Position GenPosDir(CPU_INT08U addr, CPU_INT08U direction) {
    switch(direction) {
        case 1:
            return GenPosCoord(roboStats[addr].currentPos.xPos, roboStats[addr].currentPos.yPos+1);
        case 2:
            return GenPosCoord(roboStats[addr].currentPos.xPos+1, roboStats[addr].currentPos.yPos+1);
        case 3:
            return GenPosCoord(roboStats[addr].currentPos.xPos+1, roboStats[addr].currentPos.yPos);
        case 4:
            return GenPosCoord(roboStats[addr].currentPos.xPos+1, roboStats[addr].currentPos.yPos-1);
        case 5:
            return GenPosCoord(roboStats[addr].currentPos.xPos, roboStats[addr].currentPos.yPos-1);
        case 6:
            return GenPosCoord(roboStats[addr].currentPos.xPos-1, roboStats[addr].currentPos.yPos-1);
        case 7:
            return GenPosCoord(roboStats[addr].currentPos.xPos-1, roboStats[addr].currentPos.yPos);
        case 8:
            return GenPosCoord(roboStats[addr].currentPos.xPos-1, roboStats[addr].currentPos.yPos+1);
        default:
            return GenPosCoord(roboStats[addr].currentPos.xPos, roboStats[addr].currentPos.yPos);;
    }
}

/*---------------- LostRobot() ----------------

Purpose:
If robot is lost, generate no-step step to generate Here I Am to locate robot

Parameters: Payload *ctrlOBfr, CPU_INT08U addr

Return value: none
*/

CPU_VOID LostRobot(Payload *ctrlOBfr, CPU_INT08U addr) {
    OS_ERR osErr;
  
    ctrlOBfr = Allocate();
    
    ctrlOBfr->payloadLen = PacketSize;
    ctrlOBfr->dstAddr = addr+3;
    ctrlOBfr->srcAddr = SourceAddress;
    ctrlOBfr->pktType = StepPacket;
    ctrlOBfr->cmdExt.direction = NoStep;

    OSQPost(&framerQueue, ctrlOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    ctrlOBfr = NULL;
}

/*---------------- GiveUp() ----------------

Purpose:
If robot fails to reach destination,
send error packet indicating failure to reach destination

Parameters: Payload *ctrlOBfr, CPU_INT08U addr

Return value: none
*/

CPU_VOID GiveUp(Payload *ctrlOBfr, CPU_INT08U addr) {
    OS_ERR osErr;
  
    ctrlOBfr = Allocate();
    
    ctrlOBfr->payloadLen = PacketSize;
    ctrlOBfr->dstAddr = DestinationAddress;
    ctrlOBfr->srcAddr = SourceAddress;
    ctrlOBfr->pktType = ErrorPacket;
    ctrlOBfr->cmdExt.errCode = RobotGiveUp+addr;

    OSQPost(&framerQueue, ctrlOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    ctrlOBfr = NULL;
}