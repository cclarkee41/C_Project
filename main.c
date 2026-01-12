// main.c (GTK3)
// Compile: gcc *.c -o main $(pkg-config --cflags --libs gtk+-3.0)
// After compile run ./main

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

// ---- Queue API you will implement in other files ----

Queue* createQueue(void);
void enqueue(Queue* q, const char* name, const char* reason, int prio);
int  dequeue(Queue* q, char* outName, char* outReason, int* outPrio); // return 1 if ok, 0 if empty
int  peek(Queue* q, char* outName, char* outReason, int* outPrio);    // return 1 if ok, 0 if empty
int  isEmpty(Queue* q);
void freeQueue(Queue* q);

// Optional: have queue build a string into a buffer for printing in GUI
void printQueueToString(Queue* q, char* out, size_t outSize);
// ----------------------------------------------------

typedef struct {
    GtkTextBuffer *log_buffer;
    Queue *queue;
} AppState;

static void append_log(AppState *app, const char *line) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(app->log_buffer, &end);
    gtk_text_buffer_insert(app->log_buffer, &end, line, -1);
    gtk_text_buffer_insert(app->log_buffer, &end, "\n", -1);
}

static void show_error(GtkWindow *parent, const char *msg) {
    GtkWidget *d = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s",
        msg
    );
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

static void on_quit(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;
    if (app->queue) {
        freeQueue(app->queue);
        app->queue = NULL;
    }
    gtk_main_quit();
}

static void on_print_queue(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;

    char buf[4096];
    buf[0] = '\0';

    printQueueToString(app->queue, buf, sizeof(buf));
    append_log(app, "QUEUE SNAPSHOT:");
    append_log(app, buf[0] ? buf : "(empty)");
}

static void on_check_empty(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;

    if (isEmpty(app->queue)) append_log(app, "STATUS: Queue is empty");
    else append_log(app, "STATUS: Queue has patients");
}

static void on_announce_next(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;

    char name[128], reason[256];
    int prio = 0;

    if (!peek(app->queue, name, reason, &prio)) {
        append_log(app, "NEXT: Queue empty");
        return;
    }

    char line[512];
    snprintf(line, sizeof(line), "NEXT: %s | Priority %d | Reason: %s", name, prio, reason);
    append_log(app, line);
}

static void on_care_next(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;

    char name[128], reason[256];
    int prio = 0;

    if (!dequeue(app->queue, name, reason, &prio)) {
        append_log(app, "DEQUEUE: Queue empty");
        return;
    }

    char line[512];
    snprintf(line, sizeof(line), "DEQUEUE: %s | Priority %d | Reason: %s", name, prio, reason);
    append_log(app, line);
}

static void on_admit(GtkWidget *w, gpointer user_data) {
    (void)w;
    AppState *app = (AppState*)user_data;

    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(w)));

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Admit patient",
        parent,
        GTK_DIALOG_MODAL,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Admit", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 12);
    gtk_container_add(GTK_CONTAINER(content), grid);

    GtkWidget *name_lbl = gtk_label_new("Name");
    GtkWidget *reason_lbl = gtk_label_new("Reason");
    GtkWidget *prio_lbl = gtk_label_new("Priority (1-5)");

    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *reason_entry = gtk_entry_new();

    GtkAdjustment *adj = gtk_adjustment_new(1, 1, 5, 1, 1, 0);
    GtkWidget *prio_spin = gtk_spin_button_new(adj, 1, 0);

    gtk_grid_attach(GTK_GRID(grid), name_lbl,    0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry,  1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), reason_lbl,  0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), reason_entry,1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), prio_lbl,    0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), prio_spin,   1, 2, 1, 1);

    gtk_widget_show_all(dialog);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char *reason = gtk_entry_get_text(GTK_ENTRY(reason_entry));
        int prio = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(prio_spin));

        if (!name || !*name) {
            show_error(parent, "Name is required");
            gtk_widget_destroy(dialog);
            return;
        }
        if (!reason || !*reason) {
            show_error(parent, "Reason is required");
            gtk_widget_destroy(dialog);
            return;
        }
        if (prio < 1 || prio > 5) {
            show_error(parent, "Priority must be between 1 and 5");
            gtk_widget_destroy(dialog);
            return;
        }

        enqueue(app->queue, name, reason, prio);

        char line[512];
        snprintf(line, sizeof(line), "ADMIT: %s | Priority %d | Reason: %s", name, prio, reason);
        append_log(app, line);
    }

    gtk_widget_destroy(dialog);
}

static GtkWidget* build_menu_bar(AppState *app) {
    GtkWidget *menubar = gtk_menu_bar_new();

    // Patients menu
    GtkWidget *patients_item = gtk_menu_item_new_with_label("Patients");
    GtkWidget *patients_menu = gtk_menu_new();

    GtkWidget *admit_item = gtk_menu_item_new_with_label("Admit patient");
    GtkWidget *care_item  = gtk_menu_item_new_with_label("Care next patient");
    GtkWidget *next_item  = gtk_menu_item_new_with_label("Announce next patient");

    g_signal_connect(admit_item, "activate", G_CALLBACK(on_admit), app);
    g_signal_connect(care_item,  "activate", G_CALLBACK(on_care_next), app);
    g_signal_connect(next_item,  "activate", G_CALLBACK(on_announce_next), app);

    gtk_menu_shell_append(GTK_MENU_SHELL(patients_menu), admit_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(patients_menu), care_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(patients_menu), next_item);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(patients_item), patients_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), patients_item);

    // Queue menu
    GtkWidget *queue_item = gtk_menu_item_new_with_label("Queue");
    GtkWidget *queue_menu = gtk_menu_new();

    GtkWidget *print_item = gtk_menu_item_new_with_label("Print full queue");
    GtkWidget *empty_item = gtk_menu_item_new_with_label("Check if empty");

    g_signal_connect(print_item, "activate", G_CALLBACK(on_print_queue), app);
    g_signal_connect(empty_item, "activate", G_CALLBACK(on_check_empty), app);

    gtk_menu_shell_append(GTK_MENU_SHELL(queue_menu), print_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(queue_menu), empty_item);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(queue_item), queue_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), queue_item);

    // File menu
    GtkWidget *file_item = gtk_menu_item_new_with_label("File");
    GtkWidget *file_menu = gtk_menu_new();

    GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit), app);

    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);

    return menubar;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    AppState app;
    app.queue = createQueue();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ER Patient Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(on_quit), &app);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *menubar = build_menu_bar(&app);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    GtkWidget *scroller = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scroller, TRUE, TRUE, 0);

    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);
    gtk_container_add(GTK_CONTAINER(scroller), textview);

    app.log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    append_log(&app, "SYSTEM: Session started");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
