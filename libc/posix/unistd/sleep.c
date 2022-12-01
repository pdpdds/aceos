/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <unistd.h>
#include <time.h>

unsigned int
sleep(unsigned int _seconds)
{
  clock_t start_time = clock();
  while (clock()-start_time < _seconds * CLOCKS_PER_SEC);
  return 0;
}
