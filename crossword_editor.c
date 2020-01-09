#include <string.h>

#include "crossword_editor.h"

typedef struct {
    int x;
    int y;
    Crossword *crossword;
} GridEntryCallbackData;

typedef struct {
    GtkWindow *parent;
    Crossword *crossword;
    char *filename;
} ToolButtonCallbackData;

static void entry_set_to_black(GtkWidget *entry)
{
    gtk_widget_set_sensitive(entry, FALSE);
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_css_provider_load_from_data(provider, "*{background-image:none;background-color:black;}", -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

static void grid_entry_changed_callback(GtkWidget *entry, gpointer data)
{
    GridEntryCallbackData *callback_data = data;
    char *s = gtk_entry_get_text(GTK_ENTRY(entry));
    s = g_utf8_strup(s, strlen(s));
    gtk_entry_set_text(GTK_ENTRY(entry), s);
    int x = callback_data->x;
    int y = callback_data->y;
    callback_data->crossword->content[y][x] = g_utf8_get_char(s);
}

static void tool_save_clicked_callback(GtkWidget *button, gpointer data)
{
    ToolButtonCallbackData *tool_data = data;
    Crossword *crossword= tool_data->crossword;
    if(tool_data->filename == NULL)
    {
        GtkWidget *file_chooser = gtk_file_chooser_dialog_new("Save crossword", tool_data->parent, 
                                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                "Cancel", GTK_RESPONSE_CANCEL,
                                                                "Save", GTK_RESPONSE_ACCEPT, NULL);
        int res = gtk_dialog_run(GTK_DIALOG(file_chooser));
        if(res == GTK_RESPONSE_ACCEPT)
        {
            tool_data->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
            gtk_widget_destroy(file_chooser);
        }
        else
        {
            gtk_widget_destroy(file_chooser); 
            return;
        }
    }

    crossword_save_to_file(crossword, tool_data->filename);
}

GtkWidget* crossword_editor_window_init(Crossword *crossword, char *filename)
{
    if(crossword == NULL)
        crossword = crossword_load_from_file(filename);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - crossword editor");

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkWidget *grid = gtk_grid_new();

    GtkWidget *save_button = gtk_button_new();
    GtkToolItem *save_tool_button = gtk_tool_button_new(save_button, NULL);   
    GtkWidget *save_image = gtk_image_new_from_file("icons/media-floppy.svg");
    ToolButtonCallbackData *save_tool_data = malloc(sizeof(ToolButtonCallbackData));
    save_tool_data->parent = window;
    save_tool_data->filename = filename;
    save_tool_data->crossword = crossword;
    gtk_button_set_image(GTK_BUTTON(save_button), save_image);
    gtk_tool_item_set_tooltip_text(save_tool_button, "Save");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_tool_button, -1);
    g_signal_connect(save_tool_button, "clicked", G_CALLBACK(tool_save_clicked_callback), save_tool_data);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    for(int i = 0; i < crossword->height; i++)
    {
        for(int j = 0; j < crossword->width; j++)
        {
            GtkWidget *entry = gtk_entry_new();
            GridEntryCallbackData *data = malloc(sizeof(GridEntryCallbackData));
            data->x = j;
            data->y = i;
            data->crossword = crossword;
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
            gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
            gtk_grid_attach(GTK_GRID(grid), entry, j, i, 1, 1);
            g_signal_connect(entry, "changed", G_CALLBACK(grid_entry_changed_callback), data);

            int c = crossword->content[i][j];
            if(c!= 0 && c != ' ')
            {
                char *buf = malloc(sizeof(char) * 6);
                g_unichar_to_utf8(c, buf);
                gtk_entry_set_text(GTK_ENTRY(entry), buf);
            }
            else if(c == 0)
                entry_set_to_black(entry);
        }
    }
    gtk_box_pack_start(GTK_BOX(hbox), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    return window;
}