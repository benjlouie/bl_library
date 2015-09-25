/* Threaded functions */
/*//////////////////*/

#ifndef BL_THREAD_H
#define BL_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <time.h>

/* void *busywork(void *tid); */
void bl_thread_print_test(int num_threads);
/* void *print_test(void *threadid); */
void bl_thread_join_test(int num_threads);
void bl_thread_mutex_test(int num_threads);
/* void *mutex_test(void *tid); */
void bl_thread_stack_test(int num_threads);
/* void *stack_test(void *tid); */
void bl_thread_cond_test(int num_threads);
/* void *cond_test(void *tid); */
void bl_thread_qsort(int *base, size_t size, int num_threads);
/* void *thread_qsort(void *data); */ /* Make multithreaded */

#endif
