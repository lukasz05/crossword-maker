#include "template_editor.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = get_template_editor_window();
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}