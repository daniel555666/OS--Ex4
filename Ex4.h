#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAXDATASIZE 1024 // max number of bytes we can get at once
pthread_mutex_t  lock;
pthread_mutex_t lock2;
#ifndef test
#define test
#include "stack.h"
#include "heapFunc.h"
#endif

