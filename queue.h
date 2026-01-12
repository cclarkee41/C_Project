#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Queue Queue;

Queue* createQueue(void);
void enqueue(Queue* q, const char* name, const char* reason, int prio);
int  dequeue(Queue* q, char* outName, char* outReason, int* outPrio);
int  peek(Queue* q, char* outName, char* outReason, int* outPrio);
int  isEmpty(Queue* q);
void freeQueue(Queue* q);

void printQueueToString(Queue* q, char* out, size_t outSize);

#endif
