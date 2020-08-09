/* File: MoveRobot.c

Author: Henry Chuang

Date: 4/27/2020

Purpose: Contains functions relevant to movement
*/

#include "RobotCtrl.h"

/*---------------- MoveRobot() ----------------

Purpose:
Generate step payload to send to framer

Parameters: CPU_INT08U addr, Payload *ctrlOBfr, CPU_INT08U *detourCount

Return value: none
*/

CPU_VOID MoveRobot(CPU_INT08U addr, Payload *ctrlOBfr, CPU_INT08U *detourCount) {
    OS_ERR osErr;
    
    ctrlOBfr = Allocate();

    ctrlOBfr->payloadLen = PacketSize;
    ctrlOBfr->dstAddr = addr+3;
    ctrlOBfr->srcAddr = SourceAddress;
    ctrlOBfr->pktType = StepPacket;
    roboStats[addr].stepCnt++;
    if(CheckNextSpot(addr, Step(addr))) {
        ctrlOBfr->cmdExt.direction = AvoidCollision(addr);
        (*detourCount)++;
    }
    else {
        ctrlOBfr->cmdExt.direction = Step(addr);
        *detourCount = 0;
    }
    
    OSQPost(&framerQueue, ctrlOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    ctrlOBfr = NULL;
}

/*---------------- Step() ----------------

Purpose:
Generate step direction based on destination and current position

Parameters: CPU_INT08U addr

Return value: CPU_INT08U
*/

CPU_INT08U Step(CPU_INT08U addr) {
    if(roboStats[addr].currentPos.xPos == roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos < roboStats[addr].destination.yPos)
        return NStep;
    else if(roboStats[addr].currentPos.xPos < roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos < roboStats[addr].destination.yPos)
        return NEStep;
    else if(roboStats[addr].currentPos.xPos < roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos == roboStats[addr].destination.yPos)
        return EStep;
    else if(roboStats[addr].currentPos.xPos < roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos > roboStats[addr].destination.yPos)
        return SEStep;
    else if(roboStats[addr].currentPos.xPos == roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos > roboStats[addr].destination.yPos)
        return SStep;
    else if(roboStats[addr].currentPos.xPos > roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos > roboStats[addr].destination.yPos)
        return SWStep;
    else if(roboStats[addr].currentPos.xPos > roboStats[addr].destination.xPos &&
       roboStats[addr].currentPos.yPos == roboStats[addr].destination.yPos)
        return WStep;
    else
        return NWStep;
}

/*---------------- AvoidCollision() ----------------

Purpose:
If there is another robot in the way, generate step direction allowing for minor detour

Parameters: CPU_INT08U addr

Return value: CPU_INT08U
*/

CPU_INT08U AvoidCollision(CPU_INT08U addr) {
    if(!(CheckLocation(GenPosDir(addr, DirectionLoop(Step(addr)-1)))) || NotWall(GenPosDir(addr, DirectionLoop(Step(addr)-1))))
        return DirectionLoop(Step(addr)-1);
    else if(!(CheckLocation(GenPosDir(addr, DirectionLoop(Step(addr)+1)))) || NotWall(GenPosDir(addr, DirectionLoop(Step(addr)+1))))
        return DirectionLoop(Step(addr)-1);
    else
        return NoStep;
}

/*---------------- LostRobot() ----------------

Purpose:
If robot is lost, generate no-step step to generate Here I Am to locate robot

Parameters: Payload *ctrlOBfr, CPU_INT08U addr

Return value: none
*/

CPU_INT08U DirectionLoop(CPU_INT08U direction) {
    if(direction > 8)
        return direction - 8;
    
    if(direction < 1)
        return direction + 8;
    
    return direction;
}

/*---------------- NotWall() ----------------

Purpose:
Check that a given position is not a wall

Parameters: Position pos

Return value: CPU_BOOLEAN
*/

CPU_BOOLEAN NotWall(Position pos) {
    return (pos.xPos <= 39 || pos.xPos >= 0 || pos.yPos <= 18 || pos.yPos >= 0);
}