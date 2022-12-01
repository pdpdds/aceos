/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <dirent.h>
#include <fcntl.h>
#include "dirstruc.h"

void
rewinddir(DIR *dir)
{
    /* Recompute need_fake_dot_dotdot member.  See comments in opendir.c.  */
    __set_need_fake_dot_dotdot(dir);
    dir->num_read = 0;
}
