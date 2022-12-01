/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <io.h>


/* Extra share flags that can be indicated by the user */
int __djgpp_share_flags;

int
open(const char* filename, int oflag, ...)
{
  int fd, dmode, bintext, dont_have_share;
  int should_create = (oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL);

  /* Check this up front, to reduce cost and minimize effect */
  if (should_create)
    if (__file_exists(filename))
    {
      /* file exists and we didn't want it to */
      errno = EEXIST;
      return -1;
    }

  /* figure out what mode we're opening the file in */
  bintext = oflag & (O_TEXT | O_BINARY);
  if (!bintext)
    bintext =  (O_TEXT | O_BINARY);
  if (!bintext)
    bintext = O_BINARY;

  /* DOS doesn't want to see these bits */
  oflag &= ~(O_TEXT | O_BINARY);

  dmode = (*((&oflag)+1) & S_IWUSR) ? 0 : 1;

  
  if (should_create)
    fd = _creatnew(filename, dmode, oflag & 0xff);
  else
  {
    fd = _open(filename, oflag);

    if (fd == -1)
    {
      /* It doesn't make sense to try anything else if there are no
	 more file handles available.  */
      if (errno == EMFILE || errno == ENFILE)
    	return fd;
      }

  if (fd == -1)
    return fd;	/* errno already set by _open or _creat */

  if ((oflag & O_TRUNC) && !should_create)
    if (_write(fd, 0, 0) < 0 )
    {
      _close(fd);
      return -1;
    }
}
  /* we do this last because _open and _create set it also. */
  /* force setmode() to do ioctl() for cooked/raw */
  #warning CHECK THIS
  //__file_handle_set(fd, bintext ^ (O_BINARY|O_TEXT));
  /* this will do cooked/raw ioctl() on character devices */
  setmode(fd, bintext);

  if(oflag & O_APPEND)
    lseek(fd, 0, SEEK_END);
    
  return fd;
}
