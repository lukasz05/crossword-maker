#include "template_editor.h"

#define CROSSWORD_W 10
#define CROSSWORD_H 10

#define WINDOW_W 600
#define WINDOW_H 600

static void button_callback(GtkWidget *button, gpointer data)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_css_provider_load_from_data(provider, "*{background-image:none;background-color:black;}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

GtkWidget* get_template_editor_window()
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - template editor");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_W, WINDOW_H);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    for(int i = 0; i < CROSSWORD_H; i++)
    {
        for(int j = 0; j < CROSSWORD_W; j++)
        {
            GtkWidget *button = gtk_button_new();
            g_signal_connect(button, "clicked", G_CALLBACK(button_callback), NULL);
            gtk_grid_attach(GTK_GRID(grid), button, i, j, 1, 1);
        }
    }
    gtk_container_add(GTK_CONTAINER(window), grid);
    return window;
}