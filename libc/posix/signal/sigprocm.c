/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <signal.h>
#include <errno.h>


/* A counter to know when to re-initialize the static sets.  */
static int sigprocmask_count = -1;

/* Which signals are currently blocked (initially none).  */
static sigset_t current_mask;

/* Which signals are pending (initially none).  */
sigset_t __sigprocmask_pending_signals;

/* Previous handlers to restore when the blocked signals are unblocked.  */
typedef void (*sighandler_t)(int);
static sighandler_t prev_handlers[_SIGMAX];

/* A signal handler which just records that a signal occured
   (it will be raised later, if and when the signal is unblocked).  */
static void
sig_suspender (int signo)
{
  sigaddset (&__sigprocmask_pending_signals, signo);
}

int
sigprocmask (int how, const sigset_t *new_set, sigset_t *old_set)
{
  int signo;
  sigset_t new_mask;

  /* If called for the first time, initialize.  */
  if (sigprocmask_count == -1)
    {
      sigprocmask_count = 0;
      sigemptyset (&__sigprocmask_pending_signals);
      sigemptyset (&current_mask);
      for (signo = 0; signo < _SIGMAX; signo++)
	prev_handlers[signo] = SIG_ERR;
    }

  if (old_set)
    *old_set = current_mask;

  if (new_set == 0)
    return 0;

  if (how != SIG_BLOCK && how != SIG_UNBLOCK && how != SIG_SETMASK)
    {
      errno = EINVAL;
      return -1;
    }

  sigemptyset (&new_mask);

  for (signo = 0; signo < 100; signo++)
    {
      if (sigismember (&current_mask, signo))
	sigaddset (&new_mask, signo);
      else if (sigismember (new_set, signo) && how != SIG_UNBLOCK)
	{
	  sigaddset (&new_mask, signo);

	  /* SIGKILL is silently ignored, as on other platforms.  */
	  if (signo != SIGKILL && prev_handlers[signo] == SIG_ERR)
	    prev_handlers[signo] = signal (signo, sig_suspender);
	}
      if ((   how == SIG_UNBLOCK
	      && sigismember (&new_mask, signo)
	      && sigismember (new_set, signo))
	  || (how == SIG_SETMASK
	      && sigismember (&new_mask, signo)
	      && !sigismember (new_set, signo)))
	{
	  sigdelset (&new_mask, signo);
	  if (prev_handlers[signo] != SIG_ERR)
	    {
	      signal (signo, prev_handlers[signo]);
	      prev_handlers[signo] = SIG_ERR;
	    }
	  if (sigismember (&__sigprocmask_pending_signals, signo))
	    {
	      sigdelset (&__sigprocmask_pending_signals, signo);
	      raise (signo);
	    }
	}
    }
  current_mask = new_mask;
  return 0;
}

