#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAXDATASIZE 100 // max number of bytes we can get at once
pthread_mutex_t  lock;
#ifndef test
#define test
#include "stack.h"
#endif
