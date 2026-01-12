#include "output.h"
#include "queue.h"
#include <stdio.h>

bool is_valid_priority(int p) {
    return p >= 1 && p <= 5;
}

int clamp_priority(int p) {
    if (p < 1) return 1;
    if (p > 5) return 5;
    return p;
}

bool safe_enqueue(PriorityQueue* q,
                  const char* name,
                  const char* reason,
                  int priority,
                  FILE* out)
{
    if (!q || !name || !reason) {
        if (out) fprintf(out, "[Error: invalid enqueue arguments]\n");
        return false;
    }

    if (!is_valid_priority(priority)) {
        int original = priority;
        priority = clamp_priority(priority);
        if (out) {
            fprintf(out,
                "[Warning] Priority %d is invalid. Clamped to %d.\n",
                original, priority
            );
        }
    }

    bool ok = enqueue(q, name, reason, priority);

    if (!ok) {
        if (out) fprintf(out, "[Error] Failed to enqueue patient.\n");
        return false;
    }

    if (out) {
        fprintf(out, "Patient successfully added:\n");
        Patient temp;
        snprintf(temp.name, sizeof(temp.name), "%s", name);
        snprintf(temp.reason, sizeof(temp.reason), "%s", reason);
        temp.priority = priority;
        temp.arrival = q->nextArrival - 1;
        print_patient(&temp, out);
    }

    return true;
}