#include <stddef.h>
#include "queue.h"

PriorityQueue create_queue(void) {
    PriorityQueue q;
    q.front = NULL;
    q.nextArrival = 1;
    return q;
}