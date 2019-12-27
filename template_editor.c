#include "template_editor.h"

#define WINDOW_W 600
#define WINDOW_H 600

typedef struct {
    int x;
    int y;
    Crossword *template;
} GridButtonCallbackData;

static void button_set_to_black(GtkWidget *button)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_css_provider_load_from_data(provider, "*{background-image:none;background-color:black;}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

static void button_set_to_white(GtkWidget *button)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_css_provider_load_from_data(provider, "*{background-image:none;background-color:white;}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

static void grid_button_clicked_callback(GtkWidget *button, gpointer data)
{
    GridButtonCallbackData *callback_data = data;
    int y = callback_data->y;
    int x = callback_data->x;
    Crossword *template = callback_data->template;

    if(template->content[y][x] == 0)
    {
        template->content[y][x] = ' ';
        button_set_to_white(button);
    }
    else
    {
        template->content[y][x] = 0;
        button_set_to_black(button);
    }
}

GtkWidget* template_editor_window_init(Crossword *template)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - template editor");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_W, WINDOW_H);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    for(int i = 0; i < template->height; i++)
    {
        for(int j = 0; j < template->width; j++)
        {
            GtkWidget *button = gtk_button_new();
            if(template->content[i][j] == 0)
                button_set_to_black(button);
            else
                button_set_to_white(button);
            
            GridButtonCallbackData *data = (GridButtonCallbackData*)malloc(sizeof(GridButtonCallbackData));
            data->template = template;
            data->y = i;
            data->x = j;
            g_signal_connect(button, "clicked", G_CALLBACK(grid_button_clicked_callback), data);
            gtk_grid_attach(GTK_GRID(grid), button, i, j, 1, 1);
        }
    }
    gtk_container_add(GTK_CONTAINER(window), grid);
    return window;
}