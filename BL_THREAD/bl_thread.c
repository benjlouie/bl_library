/* BL_THREAD Functions */
/*/////////////////////*/

#include "bl_thread.h"

#define QSORT_THRESHOLD 1

struct qsort_data_t {
    void *base;
    long tid;
};

void *busywork(void *tid);
void bl_thread_print_test(int num_threads);
void *print_test(void *threadid);
void bl_thread_join_test(int num_threads);
void bl_thread_mutex_test(int num_threads);
void *mutex_test(void *tid);
void bl_thread_stack_test(int num_threads);
void *stack_test(void *tid);
void bl_thread_cond_test(int num_threads);
void *cond_test(void *tid);
void bl_thread_qsort(int *base, size_t size, int num_threads);
void *thread_qsort(void *data);
int qsort_working_threads(void);

pthread_mutex_t test_mutex;
int test_mutex_num;

pthread_mutex_t stack_mutex;
int stack_index;
int *test_stack;

pthread_mutex_t cond_mutex;
pthread_cond_t test_cond;
int test_cond_num;
pthread_mutex_t cond_check_mutex;
int test_cond_check;

pthread_mutex_t qsort_stack_mutex;
sem_t qsort_sem;
char *qsort_busy_threads;
size_t qsort_stack_index;
size_t qsort_stack[4096];//[128]; /* 2 * log_2(2^64) * 8 bytes = 1024 bytes, not constant for multithreaded, FIX */


void *busywork(void *tid)
{
    int i = 0;
    double result = 0.0;
    
    printf("thread %ld starting...\n", *(long *)&tid);
    
    for(i = 0; i < 1000000; i++) {
        result += i;
    }
    printf("thread %ld done. Result = %f\n", *(long *)&tid, result);
    pthread_exit(NULL);
    return NULL;
}

void bl_thread_print_test(int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    int i = 0;
    int rc = 0;
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_print_test: creating thread #%d\n", i);
	    rc = pthread_create(&threads[i], NULL, (print_test), *(void **)&i);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	free(threads);
}

void *print_test(void *threadid)
{
    printf("this is thread %ld!\n", *(long *)&threadid);
    pthread_exit(NULL);
    return NULL;
}

void bl_thread_join_test(int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    long i = 0;
    int rc = 0;
    void *status = NULL;
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_join_test: creating thread #%ld\n", i);
	    rc = pthread_create(&threads[i], NULL, (busywork), *(void **)&i);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	
	for(i = 0; i < num_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if(rc) {
	        printf("ERROR: return code from pthread_join() is %d\n", rc);
	        exit(-1);
	    }
	    printf("bl_thread_join_test: completed join with thread %ld with status: %ld\n", i, *(long *)&status);
	}
	printf("bl_thread_join_test completed joining. Exiting...\n");
	free(threads);
}

void bl_thread_mutex_test(int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    long i = 0;
    int rc = 0;
    void *status = NULL;
	
	test_mutex_num = 0;
	pthread_mutex_init(&test_mutex, NULL);
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_mutex_test: creating thread #%ld\n", i);
	    rc = pthread_create(&threads[i], NULL, (mutex_test), *(void **)&i);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	
	for(i = 0; i < num_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if(rc) {
	        printf("ERROR: return code from pthread_mutex() is %d\n", rc);
	        exit(-1);
	    }
	    printf("bl_thread_mutex_test: completed join with thread %ld with status: %ld\n", i, *(long *)&status);
	}
	pthread_mutex_destroy(&test_mutex);
	free(threads);
	printf("bl_thread_mutex_test completed joining. Exiting...\n");
}

void *mutex_test(void *tid)
{
    int i = 0;
    
    pthread_mutex_lock(&test_mutex);
        for(i = 0; i < 100000000; i++) {
            ; /* busywork */
        }
        printf("Thread %ld, mutex_num = %d\n", *(long *)&tid, test_mutex_num);
        test_mutex_num++;
    pthread_mutex_unlock(&test_mutex);
    pthread_exit(tid);
    return NULL;
}

void bl_thread_stack_test(int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    long i = 0;
    int rc = 0;
    void *status = NULL;
	
	test_stack = malloc(sizeof(int) * (num_threads + 1));
	test_stack[0] = -1;
	stack_index = 1;
	pthread_mutex_init(&stack_mutex, NULL);
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_stack_test: creating thread #%ld\n", i);
	    rc = pthread_create(&threads[i], NULL, (stack_test), *(void **)&i);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	
	for(i = 0; i < num_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if(rc) {
	        printf("ERROR: return code from pthread_stack() is %d\n", rc);
	        exit(-1);
	    }
	    printf("bl_thread_stack_test: completed join with thread %ld with status: %ld\n", i, *(long *)&status);
	}
	free(test_stack);
	free(threads);
	pthread_mutex_destroy(&stack_mutex);
	printf("bl_thread_stack_test completed joining. Exiting...\n");
}

void *stack_test(void *tid)
{
    #define STACK_PUSH(x)   ({ test_stack[stack_index] = (x); stack_index++; })
    #define STACK_POP       ({ stack_index--; test_stack[stack_index]; })
    
    int i = 0;
    
    for(i = 0; i < 100000000; i++) {
            ; /* busywork */
    }
    pthread_mutex_lock(&stack_mutex);
        printf("thread %ld, pop: %d\n", *(long *)&tid, STACK_POP);
        STACK_PUSH(*(long *)&tid);
    pthread_mutex_unlock(&stack_mutex);
    pthread_exit(tid);
    return NULL;
}

/* Look into semaphores */ /* FIX */
void bl_thread_cond_test(int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    long i = 0;
    int rc = 0;
    void *status = NULL;
	
	pthread_mutex_init(&cond_mutex, NULL);
	pthread_mutex_init(&cond_check_mutex, NULL);
	pthread_cond_init(&test_cond, NULL);
	test_cond_num = 0;
	test_cond_check = 0;
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_cond_test: creating thread #%ld\n", i);
	    rc = pthread_create(&threads[i], NULL, (cond_test), *(void **)&i);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	
	pthread_mutex_lock(&cond_mutex);
	while(test_cond_check < num_threads)
	   ;
	   
	//pthread_cond_signal(&test_cond);
	
	for(i = 0; i < num_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if(rc) {
	        printf("ERROR: return code from pthread_cond() is %d\n", rc);
	        exit(-1);
	    }
	    printf("bl_thread_cond_test: completed join with thread %ld with status: %ld\n", i, *(long *)&status);
	}
	pthread_mutex_destroy(&cond_mutex);
	pthread_mutex_destroy(&cond_check_mutex);
	pthread_cond_destroy(&test_cond);
	printf("bl_thread_cond_test completed joining. Exiting...\n");
}

void *cond_test(void *tid) /* FIX, figure out cond variables */
{
    pthread_mutex_lock(&cond_check_mutex);
        test_cond_check++;
    pthread_mutex_unlock(&cond_check_mutex);
    
    pthread_cond_wait(&test_cond, &cond_mutex);
        printf("thead %ld\tcond_num = %d\n", *(long *)&tid, test_cond_num);
        test_cond_num++;
    pthread_mutex_lock(&cond_mutex);
    pthread_cond_signal(&test_cond);
    
    pthread_exit(NULL);
    return NULL;
}

void bl_thread_qsort(int *base, size_t size, int num_threads) /* get the semaphore working */
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    struct qsort_data_t *data;
    long i = 0;
    int rc = 0;
    void *status = NULL;
	
	qsort_stack[0] = 0;
	qsort_stack[1] = size - 1;
	qsort_stack_index = 2;
	pthread_mutex_init(&qsort_stack_mutex, NULL);
	sem_init(&qsort_sem, 0, 1);
	qsort_busy_threads = calloc(num_threads + 1, 1);
    qsort_busy_threads[num_threads] = -1;
    data = malloc(sizeof(struct qsort_data_t) * num_threads);
	
	for(i = 0; i < num_threads; i++) {
	    printf("bl_thread_qsort: creating thread #%ld\n", i);
	    data[i].base = base;
	    data[i].tid = i;
	    rc = pthread_create(&threads[i], NULL, (thread_qsort), (void *)&data[i]);
	    if(rc) {
	        printf("ERROR: return code from pthread_create is %d\n", rc);
	        exit(-1);
	    }
	}
	
	for(i = 0; i < num_threads; i++) {
	    rc = pthread_join(threads[i], &status);
	    if(rc) {
	        printf("ERROR: return code from pthread_qsort() is %d\n", rc);
	        exit(-1);
	    }
	    printf("bl_thread_qsort: completed join with thread %ld with status: %ld\n", i, *(long *)&status);
	}
	
	free(data);
	sem_destroy(&qsort_sem);
	pthread_mutex_destroy(&qsort_stack_mutex);
	free(threads);
	printf("bl_thread_qsort completed joining. Exiting...\n");
}

void *thread_qsort(void *data)
{
    #define QSORT_PUSH(s, e)    ({ do{ qsort_stack[qsort_stack_index++] = s; qsort_stack[qsort_stack_index++] = e; }while(0); })
    #define QSORT_POP           ({ qsort_stack[--qsort_stack_index]; }) /* will get end first */
    #define QSORT_SWAP(a, b)    ({ do{ int tmp =  arr[a]; arr[a] = arr[b]; arr[b] = tmp; }while(0); })
    
    size_t lo = 0, hi = 0, mid = 0;
    size_t start = 0, end = 0;
    size_t pivot_lsect = 0, pivot_hsect = 0;
    int pivot = 0;
    int *arr = (int *)((struct qsort_data_t *)data)->base;
    long tid = ((struct qsort_data_t *)data)->tid;
    
    
qsort_loop:
    /* get indexes */
    pthread_mutex_lock(&qsort_stack_mutex);
        if(qsort_stack_index == 0) {
            if(qsort_working_threads() == 1) {
                pthread_mutex_unlock(&qsort_stack_mutex);
                sem_wait(&qsort_sem); /* may cause problems, test thoroughly */
                goto qsort_loop;
            } else {
                pthread_mutex_unlock(&qsort_stack_mutex);
                pthread_exit(NULL);
            }
        } else {
            hi = QSORT_POP;
            lo = QSORT_POP;
        }
        qsort_busy_threads[tid] = 1;
    pthread_mutex_unlock(&qsort_stack_mutex);
    
    start = lo;
    end = hi;
    mid = lo + ((hi - lo) >> 1); /* (lo + hi) / 2 */
    
    /* median of three pivot, placed at arr[hi] */
    if(arr[lo] > arr[mid])
        QSORT_SWAP(lo, mid);
    if(arr[hi] > arr[mid])
        QSORT_SWAP(mid, hi);
    else if(arr[lo] > arr[hi])
        QSORT_SWAP(lo, hi);
    
    pivot = arr[hi];
    hi--;
    pivot_lsect = lo; /* one beyond section */
    pivot_hsect = hi;
    
    /* partition, |=P|<P|>P|=P| */
    while(lo <= hi) {
        if(arr[lo] > pivot) {
            
            while(lo <= hi && hi != ((size_t)0 - 1)) {
                if(arr[hi] < pivot) {
                    QSORT_SWAP(lo, hi);
                    hi--;
                    break;
                } else if(arr[hi] == pivot) {
                    QSORT_SWAP(hi, pivot_hsect);
                    pivot_hsect--;
                }
                hi--;
            }
            if(hi == ((size_t)0 - 1))
                  break;
        } else if(arr[lo] == pivot) {
            QSORT_SWAP(lo, pivot_lsect);
            pivot_lsect++;
        }
        lo++;
    }

    /* hi + 1 is the address gap where the pivots need to be placed */
    lo = hi;
    hi++;
    /* reorient partition, |<P|=P|>P| */
    while(pivot_lsect > start) {
        pivot_lsect--;
        QSORT_SWAP(lo, pivot_lsect);
        lo--;
    }
    while(pivot_hsect < end) {
        pivot_hsect++;
        QSORT_SWAP(hi, pivot_hsect);
        hi++;
    }

    /* compute sizes */
    if(lo == (size_t)0 - 1)
          pivot_lsect = 0;
    else
        pivot_lsect = (lo - start) + 1;
    pivot_hsect = (end - hi) + 1;

    /* add left and right sections onto stack */
    pthread_mutex_lock(&qsort_stack_mutex); /* optimize the sem_post stuff */
        if(pivot_lsect > pivot_hsect) {
            if(pivot_lsect > QSORT_THRESHOLD) {
                QSORT_PUSH(start, lo);
                sem_post(&qsort_sem);
            }
            if(pivot_hsect > QSORT_THRESHOLD) {
                QSORT_PUSH(hi, end);
                sem_post(&qsort_sem);
            }
        } else {
            if(pivot_hsect > QSORT_THRESHOLD) {
                QSORT_PUSH(hi, end);
                sem_post(&qsort_sem);
            }
            if(pivot_lsect > QSORT_THRESHOLD) {
                QSORT_PUSH(start, lo);
                sem_post(&qsort_sem);
            }
        }
        qsort_busy_threads[tid] = 0;
    pthread_mutex_unlock(&qsort_stack_mutex);
    
    /* loop back */
    goto qsort_loop;
    
    return NULL;
}

int qsort_working_threads(void) {
    int i = 0;
    while(qsort_busy_threads[i] != -1) {
        if(qsort_busy_threads[i] == 1) {
            return 1;
        }
        i++;
    }
    return 0;
}
