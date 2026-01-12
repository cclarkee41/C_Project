#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

//Patient data
typedef struct Patient {
    char name[50];
    char reason[100];
    int priority;// 1 (highest) .. 5 (lowest)
    unsigned long arrival; // increasing counter for FIFO ties (optional but useful)
} Patient;

//Linked list node
typedef struct Node {
    Patient data;
    struct Node* next;
} Node;

//Priority Queue
typedef struct PriorityQueue {
    Node* front;// top of queue (dequeue from here)
    unsigned long nextArrival; // increments each enqueue for FIFO ties
} PriorityQueue;

//Functions
PriorityQueue create_queue(void);
bool is_empty(const PriorityQueue* q);

bool enqueue(PriorityQueue* q, const char* name, const char* reason, int priority);
//returns true on success, false on invalid input or malloc fail

bool dequeue(PriorityQueue* q, Patient* outPatient);
// returns true if a patient removed, false if queue empty

// Peek (non-destructive) at the next patient (front of queue)
bool peek(const PriorityQueue* q, Patient* outPatient);

void free_queue(PriorityQueue* q);

#endif