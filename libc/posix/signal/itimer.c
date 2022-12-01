/* Copyright (C) 2001 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 Charles Sandmann (sandmann@clio.rice.edu)
   setitimer implmentation - used for profiling and alarm
   BUGS: ONLY ONE AT A TIME, first pass code
   This software may be freely distributed, no warranty.

   Changed to work with SIGALRM & SIGPROF by Tom Demmer.
   Gotchas:
     - It relies on uclock(), which does not work under Windows 3.X
       and sometimes under Windows 9X.
     - It screws up debuggers compiled with v2.02 or earlier, since
       debugging support before v2.03 couldn't pass signals to
       debugged programs.
     (Both of the above were true for the old version, too.)
*/

#include <libc/stubs.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>


#warning THIS FILE SHOULD MODIFIED TO WORK PROPER

#define DEFAULT_CLOCK_TICK_INTERVAL 54926

/* Applications should set this to the number of microseconds between
   timer ticks if they reprogram the system clock.  */
long __djgpp_clock_tick_interval = -1;

static uclock_t r_exp, r_rel,  /* When REAL expires & reload value */
                p_exp, p_rel;  /* When PROF expires & reload value */

static uclock_t u_now;

/* Multiply a signed 32-bit VAL by a signed 32-bit M and divide the
   64-bit intermediate result by a signed 32-bit D.  The inline
   assembly avoids slow long long arithmetics.

   Originally written by Sergey Vlasov <vsu@au.ru>, with improvements
   by Nate Eldredge <neldredge@hmc.edu>.  */
static inline long
muldiv(long val, long m, long d)
{
  __asm__ __volatile__ ("imull %2\n\t"
			"idivl %3"
			: "=a" (val) : "0" (val), "rm" (m), "rm" (d) : "edx");
  return val;
}

int
getitimer(int which, struct itimerval *value)
{
  return -1;
}

extern unsigned __djgpp_timer_countdown;
//extern __dpmi_paddr __djgpp_old_timer;
//extern int __djgpp_timer_hdlr;
static char timer_on = 0;

/* Set back IRQ2 handler to default values and disable own signal
   handler 
static void
stop_timer(void)
{
  if(!timer_on)
    return;
  __djgpp_timer_countdown = -1;
  __dpmi_set_protected_mode_interrupt_vector(8, &__djgpp_old_timer);
  timer_on = 0;
  signal(SIGTIMR, SIG_DFL);
}
*/

/* Returns the time to the next event in UCLOCK_PER_SEC u_now must be
   set by calling routine.  Return 0 if no event pending. */

static inline uclock_t
GetNextEvent(void)
{
  if (r_exp && p_exp)
     return (r_exp < p_exp ? r_exp - u_now : p_exp - u_now );
  else if (r_exp)
     return  r_exp - u_now;
  else if (p_exp)
     return p_exp - u_now;
  else
     return 0;
}

/* Handler for SIGTIMR */
static void
timer_action(int signum)
{
 
}

static void
start_timer(void)
{
 
}


int
setitimer(int which, struct itimerval *value, struct itimerval *ovalue)
{
 
}
