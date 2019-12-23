#include "template_editor.h"

GtkWidget* get_template_editor_window()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - template editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    return window;
}