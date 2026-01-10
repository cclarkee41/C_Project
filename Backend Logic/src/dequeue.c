#include "queue.h"
#include <stdlib.h>

bool dequeue(PriorityQueue* q, Patient* outPatient) {
    if (q == NULL || q->front == NULL) return false;

    Node* toRemove = q->front;
    if (outPatient != NULL) {
        *outPatient = toRemove->data;
    }

    q->front = toRemove->next;
    free(toRemove);
    return true;
}