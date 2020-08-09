/* File: AddRobot.c

Author: Henry Chuang

Date: 4/25/2020

Purpose: Contains function that creates a robot control task,
or generates an error if something is wrong
*/

#include "RobotMgr.h"
#include "RobotCtrl.h"

#define AddRobotSuccess 0x01
#define BadRobotAddressError 11
#define BadLocationError 12
#define LocationOccupiedError 13
#define RobotAlreadyExistsError 14

CPU_INT08U AddRobot(Payload *mgrIOBfr) {
    if(BadRobotAddress(mgrIOBfr))
        return BadRobotAddressError;
    
    if(BadLocation(mgrIOBfr))
        return BadLocationError;
    
    if(LocationOccupied(mgrIOBfr))
        return LocationOccupiedError;
    
    if(!NonExistentRobot(mgrIOBfr))
        return RobotAlreadyExistsError;
    
    CreateRobotCtrlTask(mgrIOBfr);
    return AddRobotSuccess;
}
