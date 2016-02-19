#include <stdio.h>
#include "bl_queue.h"

int icmp(const void *a, const void *b);
void print(void *data, void *userData);

int main(void)
{
    int arr[10];

    for(int i = 0; i < 10; i++) {
        arr[i] = i;
        
    }

    bl_queue *q = bl_queue_new();
    for(int i = 0; i < 10; i++) {
        bl_queue_enqueue(q, &arr[i]);
    }
    bl_queue_foreach(q, NULL, (print));
    
    int num = -1;
    /*
    num = *(int *)bl_queue_dequeue(q);
    bl_queue_enqueue(q, &arr[5]);
    for(int i = 0; i < 10; i++) {
        num = *(int *)bl_queue_dequeue(q);
    }
    printf("\n num = %d", num);
    bl_queue_enqueue(q, &arr[5]);
    bl_queue_enqueue(q, &arr[7]);
    */
    /*
    bl_queue_dequeue(q);
    bl_queue_dequeue(q);
    num = *(int *)bl_queue_peek(q);
    printf("\n num = %d", num);
    */
    /*
    num = 9;
    bl_queue_delete(q, &num, (icmp));
    bl_queue_dequeue(q);
    //bl_queue_enqueue(q, &num);
    bl_queue_dequeue(q);
    //bl_queue_dequeue(q);
    */

    num = 3;
    int newData = 666;
    int oldData= *(int *)bl_queue_modify(q, &num, &newData, (icmp));
    printf("\noldData = %d", oldData);
    printf("\nnow deleting...");
    bl_queue_foreach_remove(q, &newData, (print));
    
    printf("\nsize = %lu", bl_queue_size(q));
    printf("\n");
    bl_queue_foreach(q, NULL, (print));

    bl_queue_free(q);
    return 0;
}

void print(void *data, void *userData)
{
    printf("%d, ", *(int *)data);
}

int icmp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}
