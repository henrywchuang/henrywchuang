/* File: ResetMgr.c

Author: Henry Chuang

Date: 4/25/2020

Purpose: 
*/

#include "RobotMgr.h"

CPU_VOID ResetMgr(CPU_VOID) {
    NVIC_GenerateCoreReset();
    assert(FALSE);
}