#include <stdio.h>

#include "template_editor.h"
#include "crossword_editor.h"
#include "dictionary.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *menu_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(menu_window), "Crossword Maker");
    gtk_window_set_default_size(GTK_WINDOW(menu_window), 300, 200);
    gtk_window_set_position(GTK_WINDOW(menu_window),GTK_WIN_POS_CENTER);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *template_new = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(template_new), "New template");
    gtk_widget_set_margin_start(template_new, 10);
    gtk_widget_set_margin_end(template_new, 10);
    gtk_widget_set_margin_top(template_new, 10);
    gtk_box_pack_start(GTK_BOX(box), template_new, TRUE, TRUE, 0);
    GtkWidget *template_open = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(template_open), "Open template");
    gtk_widget_set_margin_start(template_open, 10);
    gtk_widget_set_margin_end(template_open, 10);
    gtk_box_pack_start(GTK_BOX(box), template_open, TRUE, TRUE, 0);
    GtkWidget *crossword_new = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(crossword_new), "New crossword");
    gtk_widget_set_margin_start(crossword_new, 10);
    gtk_widget_set_margin_end(crossword_new, 10);
    gtk_box_pack_start(GTK_BOX(box), crossword_new, TRUE, TRUE, 0);
    GtkWidget *crossword_open = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(crossword_open), "Open crossword");
    gtk_widget_set_margin_start(crossword_open, 10);
    gtk_widget_set_margin_end(crossword_open, 10);
    gtk_widget_set_margin_bottom(crossword_open, 10);
    gtk_box_pack_start(GTK_BOX(box), crossword_open, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(menu_window), box);
    gtk_widget_show_all(menu_window);

    gtk_main();
    return 0;
}