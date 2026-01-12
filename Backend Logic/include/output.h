#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include "queue.h"

/* -------- Printing Functions -------- */

// Print a single patient in readable format
void print_patient(const Patient* p, FILE* out);

// Print the entire queue from front to back
void print_queue(const PriorityQueue* q, FILE* out);

// Print a short summary of the next patient
void print_next_patient_summary(const Patient* p, FILE* out);

// Dequeue and print the patient (safe wrapper)
bool dequeue_and_print(PriorityQueue* q, FILE* out);


/* -------- Validation + Safe Wrappers -------- */

// Check if priority is within [1,5]
bool is_valid_priority(int p);

// Clamp priority into [1,5]
int clamp_priority(int p);

// Safe enqueue wrapper with validation + messages
bool safe_enqueue(PriorityQueue* q,
                  const char* name,
                  const char* reason,
                  int priority,
                  FILE* out);

// Safe dequeue wrapper
bool safe_dequeue(PriorityQueue* q, FILE* out);

#endif