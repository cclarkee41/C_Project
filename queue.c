#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Queue {
    int dummy;
};

Queue* createQueue(void) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q) q->dummy = 0;
    return q;
}

void enqueue(Queue* q, const char* name, const char* reason, int prio) {
    (void)q; (void)name; (void)reason; (void)prio;
    // Stub: real implementation later
}

int dequeue(Queue* q, char* outName, char* outReason, int* outPrio) {
    (void)q;
    // Stub: always empty
    if (outName) outName[0] = '\0';
    if (outReason) outReason[0] = '\0';
    if (outPrio) *outPrio = 0;
    return 0;
}

int peek(Queue* q, char* outName, char* outReason, int* outPrio) {
    (void)q;
    // Stub: always empty
    if (outName) outName[0] = '\0';
    if (outReason) outReason[0] = '\0';
    if (outPrio) *outPrio = 0;
    return 0;
}

int isEmpty(Queue* q) {
    (void)q;
    return 1; // Stub: always empty
}

void freeQueue(Queue* q) {
    free(q);
}

void printQueueToString(Queue* q, char* out, size_t outSize) {
    (void)q;
    if (!out || outSize == 0) return;
    snprintf(out, outSize, "(queue stub: no data yet)");
}
