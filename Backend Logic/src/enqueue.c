#include "queue.h"
#include <stdlib.h>
#include <string.h>

static bool valid_priority(int p) {
    return (p >= 1 && p <= 5);
}

bool enqueue(PriorityQueue* q, const char* name, const char* reason, int priority) {
    if (q == NULL || name == NULL || reason == NULL) return false;
    if (!valid_priority(priority)) return false;

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return false;

    // Fill patient data safely
    memset(newNode, 0, sizeof(Node));
    strncpy(newNode->data.name, name, sizeof(newNode->data.name) - 1);
    strncpy(newNode->data.reason, reason, sizeof(newNode->data.reason) - 1);
    newNode->data.priority = priority;
    newNode->data.arrival = q->nextArrival++;
    newNode->next = NULL;

    // Case 1: empty queue
    if (q->front == NULL) {
        q->front = newNode;
        return true;
    }

    // Case 2: must go before current front (higher urgency = smaller number)
    if (priority < q->front->data.priority) {
        newNode->next = q->front;
        q->front = newNode;
        return true;
    }

    /*
      Case 3: insert somewhere after front.
      RULES:
      - Keep ascending by priority number (1 then 2 then 3...)
      - For SAME priority, preserve FIFO => insert AFTER all nodes with same priority
    */
    Node* cur = q->front;

    // Move while next exists AND next is more urgent OR same priority (to keep FIFO)
    while (cur->next != NULL &&
          (cur->next->data.priority < priority ||
           cur->next->data.priority == priority)) {
        cur = cur->next;
           }

    // Insert after cur
    newNode->next = cur->next;
    cur->next = newNode;
    return true;
}