#include "template_editor.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    Crossword *template = crossword_init(10, 10);
    GtkWidget *window = template_editor_window_init(template, NULL);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}