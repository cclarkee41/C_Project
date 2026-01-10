#include <stddef.h>
#include "queue.h"

bool is_empty(const PriorityQueue* q) {
    return (q == NULL || q->front == NULL);
}