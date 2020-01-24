#include <stdbool.h>

#include "template_editor.h"

#define WINDOW_W 600
#define WINDOW_H 600

typedef struct {
    int x;
    int y;
    Crossword *template;
} GridButtonCallbackData;

typedef struct {
    GtkWindow *parent;
    GtkGrid *grid;
    Crossword *template;
    char *filename;
} ToolButtonCallbackData;

static bool dialog_request_size(GtkWindow *parent, int *width, int *height)
{
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Resize template", parent, flags, "OK", GTK_RESPONSE_ACCEPT, 
                                                    "Cancel", GTK_RESPONSE_REJECT, NULL);

    GtkWidget *width_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *width_entry = gtk_spin_button_new_with_range(1, MAX_CROSSWORD_WIDTH, 1);
    gtk_box_pack_start(GTK_BOX(width_box), width_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(width_box), width_entry, FALSE, FALSE, 5);
    gtk_widget_show_all(width_box);

    GtkWidget *height_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *height_entry = gtk_spin_button_new_with_range(1, MAX_CROSSWORD_HEIGHT, 1);
    gtk_box_pack_start(GTK_BOX(height_box), height_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(height_box), height_entry, FALSE, FALSE, 5);
    gtk_widget_show_all(height_box);

    GtkWidget *dialog_box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_box_set_spacing(GTK_BOX(dialog_box), 5);
    gtk_box_pack_start(GTK_BOX(dialog_box), width_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dialog_box), height_box, FALSE, FALSE, 0);

    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    if(result == GTK_RESPONSE_ACCEPT)
    {
        *width = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(width_entry));
        *height = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(height_entry));
        gtk_widget_destroy(dialog);
        return true;
    }
    gtk_widget_destroy(dialog);
    return false;
}

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

static void grid_refresh_buttons(GtkGrid *grid, Crossword *template, int prev_w, int prev_h)
{
    for(int i = 0; i < template->height; i++)
    {
        for(int j = 0; j < template->width; j++)
        {
            GtkWidget *button = gtk_grid_get_child_at(grid, j, i);
            if(button == NULL)
            {
                button = gtk_button_new();
                GridButtonCallbackData *data = (GridButtonCallbackData*)malloc(sizeof(GridButtonCallbackData));
                data->template = template;
                data->y = i;
                data->x = j;
                g_signal_connect(button, "clicked", G_CALLBACK(grid_button_clicked_callback), data);
                gtk_grid_attach(grid, button, j, i, 1, 1);
                gtk_widget_show(button);
            }

            if(template->content[i][j] == 0)
                button_set_to_black(button);
            else
                button_set_to_white(button);
        }
    }

    for(int i = prev_w - 1; i >= template->width; i--)
        gtk_grid_remove_column(grid, i);

    for(int j = prev_h - 1; j >= template->height; j--)
        gtk_grid_remove_row(grid, j);
}

static void tool_save_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    if(tool_data->filename == NULL)
    {
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
        GtkFileChooserNative *dialog = gtk_file_chooser_native_new("Save crossword", tool_data->parent, 
                                                                         action, "_Save", "_Cancel");

        int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog));
        if(res == GTK_RESPONSE_ACCEPT)
        {
            tool_data->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            g_object_unref(dialog);
        }
        else
        {
            g_object_unref(dialog);
        }
    }
    crossword_save_to_file(template, tool_data->filename);
}

static void tool_clear_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    crossword_set_template_white(template);
    grid_refresh_buttons(tool_data->grid, template, template->width, template->height);
}

static void tool_invert_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    crossword_invert_template(template);
    grid_refresh_buttons(tool_data->grid, template, template->width, template->height);
}

static void tool_resize_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *template = tool_data->template;
    int w = template->width;
    int h = template->height;

    int new_w = 1;
    int new_h = 1;
    if(dialog_request_size(tool_data->parent, &new_w, &new_h))
    {
        crossword_resize(template, new_w, new_h);
        grid_refresh_buttons(tool_data->grid, template, w, h);
    }
    gtk_window_resize(tool_data->parent, 1, 1);
}


GtkWidget* template_editor_window_init(Crossword *template, char *filename)
{
    if(template == NULL)
        template = crossword_load_from_file(filename);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - template editor");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkWidget *grid = gtk_grid_new();
    
    GtkWidget *save_button = gtk_button_new();
    GtkToolItem *save_tool_button = gtk_tool_button_new(save_button, NULL);   
    GtkWidget *save_image = gtk_image_new_from_file("icons/media-floppy.svg");
    ToolButtonCallbackData *save_tool_data = malloc(sizeof(ToolButtonCallbackData));
    save_tool_data->parent = GTK_WINDOW(window);
    save_tool_data->filename = filename;
    save_tool_data->grid = GTK_GRID(grid);
    save_tool_data->template = template;
    gtk_button_set_image(GTK_BUTTON(save_button), save_image);
    gtk_tool_item_set_tooltip_text(save_tool_button, "Save");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_tool_button, -1);
    g_signal_connect(save_tool_button, "clicked", G_CALLBACK(tool_save_clicked_callback), save_tool_data);
    
    GtkToolItem *separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, -1);

    GtkWidget *clear_button = gtk_button_new();
    GtkToolItem *clear_tool_button = gtk_tool_button_new(clear_button, NULL);   
    GtkWidget *clear_image = gtk_image_new_from_file("icons/gimp-grid.svg");
    ToolButtonCallbackData *clear_tool_data = malloc(sizeof(ToolButtonCallbackData));
    clear_tool_data->grid = GTK_GRID(grid);
    clear_tool_data->template = template;
    gtk_button_set_image(GTK_BUTTON(clear_button), clear_image);
    gtk_tool_item_set_tooltip_text(clear_tool_button, "Set all cells to white");
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

    GtkWidget *resize_button = gtk_button_new();
    GtkToolItem *resize_tool_button = gtk_tool_button_new(resize_button, NULL);   
    GtkWidget *resize_image = gtk_image_new_from_file("icons/gimp-resize.svg");
    ToolButtonCallbackData *resize_tool_data = malloc(sizeof(ToolButtonCallbackData));
    resize_tool_data->parent = GTK_WINDOW(window);
    resize_tool_data->grid = GTK_GRID(grid);
    resize_tool_data->template = template;
    gtk_button_set_image(GTK_BUTTON(resize_button), resize_image);
    gtk_tool_item_set_tooltip_text(resize_tool_button, "Resize");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), resize_tool_button, -1);
    g_signal_connect(resize_tool_button, "clicked", G_CALLBACK(tool_resize_clicked_callback), resize_tool_data);

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
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    return window;
}