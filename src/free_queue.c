#include "queue.h"
#include <stdlib.h>

void free_queue(PriorityQueue* q) {
    if (q == NULL) return;

    Node* cur = q->front;
    while (cur != NULL) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    q->front = NULL;
}