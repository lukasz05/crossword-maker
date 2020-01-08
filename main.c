#include "template_editor.h"
#include "crossword_editor.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    Crossword *crossword = crossword_init(10, 10);
    GtkWidget *window = crossword_editor_window_init(NULL, "test.txt");
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}