/*
  Name: 		Atomic Operations
  Author: 			- Samuel
  Description: 	This file contains neccessary routines to do atomic operations(add, sub) on a integer
  Date: 30-Nov-03 21:11   Copied from the Linux Source
*/
#ifndef __ATOMIC__H
#define __ATOMIC__H

#ifdef CONFIG_SMP
	#define LOCK "lock ; "
#else
	#define LOCK ""
#endif
/*
	Make sure gcc doesn’t try to be clever and move things around* on us. We need to use _exactly_ the address the user gave us,
	not some alias that contains the same information.
*/
typedef struct { volatile int Counter; } ATOMIC;

/**
	AtomicAdd - Add integer to atomic variable
	i - integer value to add
	v - pointer of type ATOMIC

	Atomically adds @i to @v. Note that the guaranteed useful range of an atomic_t is only 24 bits.
*/
static __inline__ void AtomicAdd(int i, ATOMIC *v)
{
    __asm__ __volatile__
        ( 
        LOCK "addl %1,%0"
        :"=m" (v->Counter)
        :"ir" (i), "m" (v->Counter)
        );
}
/**
	AtomicSub - Subtract the atomic variable
	i - Integer value to subtract
	v - Pointer of type atomic_t

	Atomically subtracts @i from @v. Note that the guaranteed useful range of an atomic_t is only 24 bits.
*/
static __inline__ void AtomicSub(int i, ATOMIC *v)
{
    __asm__ __volatile__
        (
        LOCK "subl %1,%0"
        :"=m" (v->Counter)
        :"ir" (i), "m" (v->Counter)
        );
}


#endif

