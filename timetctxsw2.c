// Copyright (C) 2010  Benoit Sigoure
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "timetctxsw2.h"
#include <bits/time.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

static void* thread(void*ctx) {
  int* nSwitchesPtr = (int*)ctx;
  int nSwitches = *nSwitchesPtr;
  for (int i = 0; i < nSwitches; i++)
      sched_yield();
  return NULL;
}

int thread_loop(int nSwitches) {
  int halfSwitches = nSwitches/2;

  struct sched_param param;
  param.sched_priority = 1;
  if (sched_setscheduler(getpid(), SCHED_FIFO, &param))
    fprintf(stderr, "sched_setscheduler(): %s\n", strerror(errno));

  struct timespec ts;
  pthread_t thd;
  if (pthread_create(&thd, NULL, thread, &halfSwitches)) {
    return -1;
  }

  for (int i = 0; i < halfSwitches; i++)
    sched_yield();
  
  return 0;
}
