/*=============== M e m M g r . h ===============*/

/*
BY:	George Cheney
		16.472 / 16.572 Embedded Real Time Systems
		Electrical and Computer Engineering Dept.
		UMASS Lowell

PURPOSE
Interface to MemMgr.c.

CHANGES
03-24-2011  - Release to class.
*/

#ifndef MEMMGR_H
#define MEMMGR_H

/*----- c o n s t a n t    d e f i n i t i o n s -----*/

/* Total number of buffers in buffer pool */
#define PoolSize	10

/*----- g l o b a l s -----*/

/* The buffer pool */
extern OS_MEM bfrPool;

/*----- f u n c t i o n    p r o t o t y p e s -----*/

CPU_VOID InitMemMgr(CPU_VOID);
Payload *Allocate(CPU_VOID);
CPU_VOID Free(Payload *bfr);

#endif