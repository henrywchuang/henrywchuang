/* File: ForwardCommand.c

Author: Henry Chuang

Date: 4/27/2020

Purpose: Contains function to either forward commands to appropriate message queue
or return an error
*/

#include "RobotMgr.h"
#include "RobotCtrl.h"

#define BadRobotAddressError 1
#define NonExistentRobotError 2
#define BadLocationError 3
#define StopCommand 0x05

CPU_INT08U ForwardCommand(Payload *mgrIOBfr) {
    OS_ERR osErr;
    CPU_INT08U retVal = mgrIOBfr->pktType;

    if(BadRobotAddress(mgrIOBfr))
        return (retVal*10+BadRobotAddressError);
    
    if(NonExistentRobot(mgrIOBfr))
        return (retVal*10+NonExistentRobotError);
    
    if(retVal != StopCommand)
        if(BadLocation(mgrIOBfr))
            return (retVal*10+BadLocationError);

    OSQPost(&(roboStats[mgrIOBfr->cmdExt.mgrCmd.robotAddr-3].myQueue), mgrIOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
    return retVal;
}