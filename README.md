ER Patient Management System (GTK + Priority Queue)

What you have
- src/main.c: GTK GUI (menu + scrollable log). GUI is built entirely in main.c.
- include/queue.h + src/*.c: priority queue implemented with linked list (dynamic memory).
- include/output.h + src/output.c + src/queue_to_string.c: printing helpers.
- Project_Report_Template.pdf: report template you can complete with names, screenshots, and results.

Menu functions
- Patients -> Admit patient
- Patients -> Care next patient (dequeue)
- Patients -> Announce next patient (peek)
- Queue -> Print full queue
- Queue -> Check if empty
- File -> Quit

Notes for compliance with the assignment
- Priority order: 1 (highest urgency) to 5 (lowest urgency).
- FIFO for same priority is preserved.
- Dequeue always removes only from the front.
- Output is shown in the GUI log (GtkTextView).

Build (example)
- Use pkg-config for GTK3 includes and libs.
- Ensure you add -Iinclude so the compiler finds the project headers.
