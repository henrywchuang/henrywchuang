/* File: RobotCtrl.h

Author: Henry Chuang

Date: 4/22/2020

Purpose: Contains data and function declarations for robot control task
*/

#ifndef ROBOTCTRL_H
#define ROBOTCTRL_H

#include "includes.h"
#include "Buffer.h"
#include "MemMgr.h"
#include "assert.h"
#include "RobotMgr.h"

/* Size of the Robot Control task stack */
#define	CTRL_STK_SIZE     256

#define HereIAmTimeout 1000                     // Timeout for Here I Am check

#ifndef MaxRobots
#define MaxRobots 13
#endif

#define NoStep 0
#define NStep 1
#define NEStep 2
#define EStep 3
#define SEStep 4
#define SStep 5
#define SWStep 6
#define WStep 7
#define NWStep 8

#define RobotGiveUp 100
#define PacketSize 5
#define SourceAddress 2
#define DestinationAddress 1
#define StepPacket 0x07
#define ErrorPacket 0x0B

typedef struct {
    CPU_INT08U address;                                 // Robot address 3 <= address <= 15
    CPU_BOOLEAN lost;                                   // True if a robot is lost
    CPU_BOOLEAN dead;                                   // True if a robot has died in a collision
    Position currentPos;                                // Robot's current position
    Position destination;                               // Robot's destination
    CPU_INT32U stepCnt;                                 // Step number of moving robot
    OS_Q myQueue;                                       // Passes buffers from robot manager to robot controller
    OS_Q myMBox;                                        // Passes Here I Am messages to robot controller
    CPU_STK ctrlStk[CTRL_STK_SIZE];                     // Stack space for Process task stack
} RobotStatus;

#define StopTimeout 10

// Robot Control Task Control Block
extern OS_TCB ctrlTCB[MaxRobots];

// Robot Control Status
extern RobotStatus roboStats[MaxRobots];

// Restricts access to roboStats
extern OS_SEM statusAccess;

// Call this function to create the control task
CPU_VOID CreateRobotCtrlTask(Payload *buffer);

// Robot control task
CPU_VOID RobotCtrl(CPU_VOID *pointer);

// Step
CPU_INT08U Step(CPU_INT08U addr);

// Move robot
CPU_VOID MoveRobot(CPU_INT08U addr, Payload *ctrlOBfr, CPU_INT08U *detourCount);

// Collision Avoidance Handler
CPU_INT08U AvoidCollision(CPU_INT08U addr);

// Loop direction
CPU_INT08U DirectionLoop(CPU_INT08U direction);

// Equal position
CPU_BOOLEAN EqualPosition(Position a, Position b);

// Check Location
CPU_BOOLEAN CheckLocation(Position pos);

// Check Next Spot
CPU_BOOLEAN CheckNextSpot(CPU_INT08U addr, CPU_INT08U step);

// Generate Position based on Coordinates
Position GenPosCoord(CPU_INT08S xPos, CPU_INT08S yPos);

// Generate Position based on Direction
Position GenPosDir(CPU_INT08U addr, CPU_INT08U direction);

// Lost Robot Handler
CPU_VOID LostRobot(Payload *ctrlOBfr, CPU_INT08U addr);

// Check to see if not a wall
CPU_BOOLEAN NotWall(Position pos);

// Robot Give Up
CPU_VOID GiveUp(Payload *ctrlOBfr, CPU_INT08U addr);

#endif