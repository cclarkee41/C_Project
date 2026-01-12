#include "queue.h"

bool peek(const PriorityQueue* q, Patient* outPatient) {
    if (!q || !q->front) return false;
    if (outPatient) *outPatient = q->front->data;
    return true;
}
