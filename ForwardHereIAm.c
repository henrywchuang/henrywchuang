/* File: ForwardHereIAm.c

Author: Henry Chuang

Date: 4/27/2020

Purpose: Contains function to forward Here I Am messages to appropriate mailbox
*/

#include "RobotMgr.h"
#include "RobotCtrl.h"

CPU_VOID ForwardHereIAm(Payload *mgrIOBfr) {
    OS_ERR osErr;
  
    OSQPost(&(roboStats[mgrIOBfr->srcAddr-3].myMBox), mgrIOBfr, sizeof(Payload), OS_OPT_POST_FIFO, &osErr);
    assert(osErr == OS_ERR_NONE);
}