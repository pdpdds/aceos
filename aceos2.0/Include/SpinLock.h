/*
  Name: 		Kernel Spin Lock
  Author: 			- Samuel (samueldotj@gmail.com)
  Date: 21-Jun-05 20:19   Copied from the Linux Source
*/
#ifndef __SPIN_LOCK__H
#define __SPIN_LOCK__H

typedef struct { volatile unsigned int lock; } spinlock_t;
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 1 }
#define spin_lock_init(x) do { *(x) = SPIN_LOCK_UNLOCKED; } while(0)

#define spin_is_locked(x) (*(volatile signed char*)(&(x)->lock) <= 0)
#define spin_unlock_wait(x) do { barrier(); } while(spin_is_locked(x))

#warning "get the original definition from Net or Linux source"
#define LOCK_SECTION_START(x) ""
#define LOCK_SECTION_END ""

#define spin_lock_string \
    "\n1:\t" \
    "lock ; decb %0\n\t" \
    "js 2f\n" \
    LOCK_SECTION_START("") \
    "2:\t" \
    "cmpb $0,%0\n\t" \
    "rep;nop\n\t" \
    "jle 2b\n\t" \
    "jmp 1b\n" \
    LOCK_SECTION_END

#define spin_unlock_string \
    "movb $1,%0" \
    :"=m" (lock->lock) : : "memory"

static inline void spin_unlock(spinlock_t *lock)
{
    __asm__ __volatile__( spin_unlock_string );
}
static inline int spin_trylock(spinlock_t *lock)
{
    char oldval;
    __asm__ __volatile__(
        "xchgb %b0,%1"
        :"=q" (oldval), "=m" (lock->lock)
        :"0" (0) : "memory");
    return oldval > 0;
}
static inline void spin_lock(spinlock_t *lock)
{
    __asm__ __volatile__( spin_lock_string :"=m" (lock->lock) : : "memory");
}


#endif
