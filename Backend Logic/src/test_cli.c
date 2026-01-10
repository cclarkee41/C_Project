#include <stdio.h>
#include "queue.h"
#include "output.h"

int main(void) {
    PriorityQueue q = create_queue();

    safe_enqueue(&q, "Alice", "Headache", 3, stdout);
    safe_enqueue(&q, "Bob", "Chest pain", 1, stdout);
    safe_enqueue(&q, "Charlie", "Sprained ankle", 4, stdout);
    safe_enqueue(&q, "InvalidPriority", "Testing", 99, stdout);

    printf("\n--- Queue State ---\n");
    print_queue(&q, stdout);

    printf("\n--- Dequeueing ---\n");
    while (!is_empty(&q)) {
        safe_dequeue(&q, stdout);
    }

    printf("\n--- Final Queue ---\n");
    print_queue(&q, stdout);

    free_queue(&q);
    return 0;
}