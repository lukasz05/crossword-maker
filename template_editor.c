#include "template_editor.h"

#define WINDOW_W 600
#define WINDOW_H 600

typedef struct {
    int x;
    int y;
    Crossword *template;
} GridButtonCallbackData;

typedef struct {
    GtkGrid *grid;
    Crossword *template;
} ToolButtonCallbackData;


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

static void grid_refresh_buttons(GtkGrid *grid, Crossword *template)
{
    for(int i = 0; i < template->height; i++)
    {
        for(int j = 0; j < template->width; j++)
        {
            GtkWidget *button = gtk_grid_get_child_at(grid, i, j);
            if(template->content[i][j] == 0)
                button_set_to_black(button);
            else
                button_set_to_white(button);
        }
    }
}

static void tool_clear_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    crossword_set_template_white(template);
    grid_refresh_buttons(tool_data->grid, template);
}

static void tool_invert_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    crossword_invert_template(template);
    grid_refresh_buttons(tool_data->grid, template);
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

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkWidget *grid = gtk_grid_new();
    
    GtkWidget *save_button = gtk_button_new();
    GtkToolItem *save_tool_button = gtk_tool_button_new(save_button, NULL);   
    GtkWidget *save_image = gtk_image_new_from_file("icons/media-floppy.svg");
    gtk_button_set_image(GTK_BUTTON(save_button), save_image);
    gtk_tool_item_set_tooltip_text(save_tool_button, "Save");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_tool_button, -1);
    
    GtkToolItem *separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, -1);

    GtkWidget *clear_button = gtk_button_new();
    GtkToolItem *clear_tool_button = gtk_tool_button_new(clear_button, NULL);   
    GtkWidget *clear_image = gtk_image_new_from_file("icons/gimp-grid.svg");
    ToolButtonCallbackData *clear_tool_data = malloc(sizeof(ToolButtonCallbackData));
    clear_tool_data->grid = GTK_GRID(grid);
    clear_tool_data->template = template;
    gtk_button_set_image(GTK_BUTTON(clear_button), clear_image);
    gtk_tool_item_set_tooltip_text(clear_tool_button, "Set all to white");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), clear_tool_button, -1);
    g_signal_connect(clear_tool_button, "clicked", G_CALLBACK(tool_clear_clicked_callback), clear_tool_data);

    GtkWidget *invert_button = gtk_button_new();
    GtkToolItem *invert_tool_button = gtk_tool_button_new(invert_button, NULL);   
    GtkWidget *invert_image = gtk_image_new_from_file("icons/gimp-invert.svg");
    ToolButtonCallbackData *invert_tool_data = malloc(sizeof(ToolButtonCallbackData));
    invert_tool_data->grid = GTK_GRID(grid);
    invert_tool_data->template = template;
    gtk_button_set_image(GTK_BUTTON(invert_button), invert_image);
    gtk_tool_item_set_tooltip_text(invert_tool_button, "Invert colors");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), invert_tool_button, -1);
    g_signal_connect(invert_tool_button, "clicked", G_CALLBACK(tool_invert_clicked_callback), invert_tool_data);

    gtk_box_pack_start(GTK_BOX(box), toolbar, FALSE, FALSE, 0);

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
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    return window;
}