#include "crossword_editor.h"

static void entry_set_to_black(GtkWidget *entry)
{
    gtk_widget_set_sensitive(entry, FALSE);
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_css_provider_load_from_data(provider, "*{background-image:none;background-color:black;}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

GtkWidget* crossword_editor_window_init(Crossword *crossword, char *filename)
{
    if(crossword == NULL)
        crossword = crossword_load_from_file(filename);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - crossword editor");

    GtkGrid *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    for(int i = 0; i < crossword->height; i++)
    {
        for(int j = 0; j < crossword->width; j++)
        {
            GtkWidget *entry = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
            gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
            gtk_grid_attach(GTK_GRID(grid), entry, j, i, 1, 1);

            if(crossword->content[i][j] == 0)
                entry_set_to_black(entry);
        }
    }
    gtk_container_add(GTK_CONTAINER(window), grid);

    return window;
}