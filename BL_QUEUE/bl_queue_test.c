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
    printf("\n\n");
    
    int num = -1;
    num = *(int *)bl_queue_dequeue(q);
    bl_queue_enqueue(q, &arr[5]);
    printf("dequed once and enqueued %d\n", arr[5]);
    bl_queue_foreach(q, NULL, (print));
    printf("\n\n");
    
    bl_queue_dequeue(q);
    bl_queue_dequeue(q);
    int peekRet = *(int *)bl_queue_peek(q);
    printf("dequeued twice, peek = %d\n", peekRet);
    bl_queue_foreach(q, NULL, (print));
    printf("\n\n");
    
    num = 8;
    int removeRet = *(int *)bl_queue_remove(q, &num, (icmp));
    printf("removed %d\n", removeRet);
    printf("size = %lu\n", bl_queue_size(q));
    bl_queue_foreach(q, NULL, (print));
    printf("\n\n");
    

    num = 4;
    int newData = 666;
    printf("modifying %d to %d\n", num, newData);
    int oldData = *(int *)bl_queue_modify(q, &num, &newData, (icmp));
    printf("oldData = %d\n", oldData);
    bl_queue_foreach(q, NULL, (print));
    printf("\n\n");
    
    
    printf("now deleting... ");
    bl_queue_foreach_remove(q, &newData, (print));
    printf("\n");
    
    printf("size = %lu\n", bl_queue_size(q));
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
