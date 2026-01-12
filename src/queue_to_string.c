#include "output.h"
#include <stdio.h>
#include <string.h>

static void safe_append(char* out, size_t outSize, size_t* used, const char* s) {
    if (!out || outSize == 0 || !used) return;
    if (*used >= outSize - 1) return;

    size_t remaining = outSize - 1 - *used;
    size_t n = strnlen(s, remaining);
    memcpy(out + *used, s, n);
    *used += n;
    out[*used] = '\0';
}

void queue_to_string(const PriorityQueue* q, char* out, size_t outSize) {
    if (!out || outSize == 0) return;
    out[0] = '\0';

    if (!q || !q->front) {
        snprintf(out, outSize, "(empty)");
        return;
    }

    size_t used = 0;
    const Node* cur = q->front;
    int idx = 1;

    while (cur) {
        char line[256];
        snprintf(line, sizeof(line), "%d) %s | P%d | %s\n",
                 idx,
                 cur->data.name,
                 cur->data.priority,
                 cur->data.reason);

        safe_append(out, outSize, &used, line);
        if (used >= outSize - 1) break;

        cur = cur->next;
        idx++;
    }
}
