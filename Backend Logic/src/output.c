#include "output.h"
#include "queue.h"
#include <stdio.h>

void print_patient(const Patient* p, FILE* out) {
    if (!p || !out) return;

    fprintf(out, "-----------------------------\n");
    fprintf(out, "Name     : %s\n", p->name);
    fprintf(out, "Reason   : %s\n", p->reason);
    fprintf(out, "Priority : %d\n", p->priority);
    fprintf(out, "Arrival# : %lu\n", p->arrival);
    fprintf(out, "-----------------------------\n");
}

void print_queue(const PriorityQueue* q, FILE* out) {
    if (!out) return;

    if (!q || is_empty(q)) {
        fprintf(out, "[Queue is empty]\n");
        return;
    }

    fprintf(out, "===== Current Queue =====\n");

    const Node* cur = q->front;
    int pos = 1;

    while (cur) {
        fprintf(out, "Position %d:\n", pos++);
        print_patient(&cur->data, out);
        cur = cur->next;
    }

    fprintf(out, "===== End of Queue =====\n");
}

void print_next_patient_summary(const Patient* p, FILE* out) {
    if (!p || !out) return;

    fprintf(out,
        "Next: %s (priority %d) - %s\n",
        p->name, p->priority, p->reason
    );
}

bool dequeue_and_print(PriorityQueue* q, FILE* out) {
    if (!out) return false;

    if (!q || is_empty(q)) {
        fprintf(out, "[No patients to attend. Queue is empty.]\n");
        return false;
    }

    Patient p;
    if (!dequeue(q, &p)) {
        fprintf(out, "[Error: dequeue failed]\n");
        return false;
    }

    fprintf(out, "Attending patient:\n");
    print_patient(&p, out);
    return true;
}

bool safe_dequeue(PriorityQueue* q, FILE* out) {
    return dequeue_and_print(q, out);
}