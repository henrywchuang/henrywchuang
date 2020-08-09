/* File: RobotMgr.h

Author: Henry Chuang

Date: 4/18/2020

Purpose: Contains relevant data and function declarations for robot manager
*/

#ifndef ROBOTMGR_H
#define ROBOTMGR_H

#include "includes.h"
#include "Buffer.h"
#include "MemMgr.h"
#include "assert.h"
#include "Framer.h"
#include "PktParser.h"
#include "RobotCtrl.h"
#include "SerIODriver.h"

#define MaxRobots 13
#define ACKMessageType 0x0A

// Call this function to create the manager task
CPU_VOID CreateRobotMgrTask(CPU_VOID);

// Robot Manager task
CPU_VOID RobotMgr(CPU_VOID *pointer);

// Creates replies
CPU_VOID ReplyHandler(Payload *mgrIOBfr, CPU_INT08U errorVal);

// Creates replies
CPU_VOID CommandHandler(Payload *mgrIOBfr);

// Check if robot address is allowed
CPU_BOOLEAN BadRobotAddress(Payload *mgrIOBfr);

// Check if robot exists
CPU_BOOLEAN NonExistentRobot(Payload *mgrIOBfr);

// Check if location is bad
CPU_BOOLEAN BadLocation(Payload *mgrIOBfr);

// Check if location is occupied
CPU_BOOLEAN LocationOccupied(Payload *mgrIOBfr);

// Reset Robot Manager
CPU_VOID ResetMgr(CPU_VOID);

// Add robot
CPU_INT08U AddRobot(Payload *mgrIOBfr);

// Forward command to robot controller
CPU_INT08U ForwardCommand(Payload *mgrIOBfr);

// Forward Here I Am reply to robot controller
CPU_VOID ForwardHereIAm(Payload *mgrIOBfr);

#endif