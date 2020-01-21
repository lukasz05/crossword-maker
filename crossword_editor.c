#include <string.h>

#include "crossword_editor.h"
#include "dictionary.h"

typedef struct {
    int x;
    int y;
} LastActiveEntryPos;

typedef struct {
    int x;
    int y;
    Crossword *crossword;
} GridEntryEditCallbackData;

typedef struct {
    int x;
    int y;
    LastActiveEntryPos *pos;
    int *orientation;
    Crossword *crossword;
    Dictionary *dictionary;
    GtkTreeView *tree_view;
} GridEntryFocusCallbackData;

typedef struct {
    LastActiveEntryPos *pos;
    int *orientation;
    int radio_orientation;
    Crossword *crossword;
    Dictionary *dictionary;
    GtkTreeView *tree_view;
} RadioButtonCallbackData;

typedef struct {
    GtkWindow *parent;
    Crossword *crossword;
    char *filename;
} ToolButtonCallbackData;

static GtkTreeModel* get_tree_model(List *suggestions)
{
    GtkListStore *model = gtk_list_store_new(1, G_TYPE_STRING);
    if(suggestions != NULL)
    {
        ListNode *L = suggestions->front;
        while(L != NULL)
        {
            gtk_list_store_insert_with_values(model, NULL, -1, 0, L->str, -1);
            L = L->next;
        }
    }
    return GTK_TREE_MODEL(model);   
}

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
    GridEntryEditCallbackData *callback_data = data;
    char *s = gtk_entry_get_text(GTK_ENTRY(entry));
    s = g_utf8_strup(s, strlen(s));
    gtk_entry_set_text(GTK_ENTRY(entry), s);
    int x = callback_data->x;
    int y = callback_data->y;
    callback_data->crossword->content[y][x] = g_utf8_get_char(s);
}

static gboolean grid_entry_button_press_callback(GtkWidget *entry, GdkEvent *event, gpointer data)
{
    GridEntryFocusCallbackData *callback_data = data;
    callback_data->pos->x = callback_data->x;
    callback_data->pos->y = callback_data->y;
    char *pattern = crossword_get_word_pattern(callback_data->crossword, callback_data->x, callback_data->y, 
                                               *(callback_data->orientation));
    
    List *suggestions = dictionary_find_words(callback_data->dictionary, pattern);
    GtkTreeModel *model = get_tree_model(suggestions);
    gtk_tree_view_set_model(callback_data->tree_view, model);
    g_object_unref(model);
    return FALSE;
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

static void radio_button_clicked_callback(GtkWidget *radio, gpointer data)
{
    RadioButtonCallbackData *callback_data = data;
    int x = callback_data->pos->x;
    int y = callback_data->pos->y;
    *(callback_data->orientation) = callback_data->radio_orientation;
    char *pattern = crossword_get_word_pattern(callback_data->crossword, x, y, *(callback_data->orientation));
    List *suggestions = dictionary_find_words(callback_data->dictionary, pattern);
    GtkTreeModel *model = get_tree_model(suggestions);
    gtk_tree_view_set_model(callback_data->tree_view, model);
    g_object_unref(model);
}

GtkWidget* crossword_editor_window_init(Crossword *crossword, char *filename)
{
    if(crossword == NULL)
        crossword = crossword_load_from_file(filename);

    Dictionary *dictionary = dictionary_load_from_file("words.txt");
    LastActiveEntryPos *last_pos = malloc(sizeof(LastActiveEntryPos));
    int *orientation = malloc(sizeof(int));
    *orientation = 0;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Crossword Maker - crossword editor");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
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

    GtkWidget *sidebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *suggestions_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(suggestions_label), "<b>Suggestions</b>");
    gtk_widget_set_margin_start(suggestions_label, 5);
    gtk_widget_set_margin_end(suggestions_label, 5);
    gtk_box_pack_start(GTK_BOX(sidebox), suggestions_label, FALSE, FALSE, 0);


    GtkWidget *tree_view = gtk_tree_view_new();

    RadioButtonCallbackData *hdata = malloc(sizeof(RadioButtonCallbackData));
    hdata->crossword = crossword;
    hdata->dictionary = dictionary;
    hdata->orientation = orientation;
    hdata->pos = last_pos;
    hdata->radio_orientation = 0;
    hdata->tree_view = tree_view;
    RadioButtonCallbackData *vdata = malloc(sizeof(RadioButtonCallbackData));
    vdata->crossword = crossword;
    vdata->dictionary = dictionary;
    vdata->orientation = orientation;
    vdata->pos = last_pos;
    vdata->radio_orientation = 1;
    vdata->tree_view = tree_view;

    GtkWidget *hradio = gtk_radio_button_new_with_label(NULL, "Horizontal");
    gtk_widget_set_margin_top(hradio, 5);
    gtk_widget_set_margin_start(hradio, 5);
    gtk_widget_set_margin_end(hradio, 5);
    gtk_box_pack_start(GTK_BOX(sidebox), hradio, FALSE, FALSE, 0);
    g_signal_connect(hradio, "toggled", G_CALLBACK(radio_button_clicked_callback), hdata);
    GtkWidget *vradio = gtk_radio_button_new_with_label(gtk_radio_button_get_group(hradio), "Vertical");
    gtk_widget_set_margin_start(vradio, 5);
    gtk_widget_set_margin_end(vradio, 5);
    gtk_box_pack_start(GTK_BOX(sidebox), vradio, FALSE, FALSE, 0);
    g_signal_connect(vradio, "toggled", G_CALLBACK(radio_button_clicked_callback), vdata);

    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("text", gtk_cell_renderer_text_new(),
                                                                          "text", 0, NULL);
    gtk_tree_view_column_set_min_width(column, 150);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), FALSE);
    gtk_widget_set_margin_top(tree_view, 5);
    gtk_widget_set_margin_start(tree_view, 5);
    gtk_widget_set_margin_end(tree_view, 5);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scroll), 150);
    gtk_container_add(GTK_CONTAINER(scroll), tree_view);
    gtk_box_pack_start(GTK_BOX(sidebox), scroll, TRUE, TRUE, 0);

    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    for(int i = 0; i < crossword->height; i++)
    {
        for(int j = 0; j < crossword->width; j++)
        {
            GtkWidget *entry = gtk_entry_new();
            GridEntryEditCallbackData *changed_data = malloc(sizeof(GridEntryEditCallbackData));
            changed_data->x = j;
            changed_data->y = i;
            changed_data->crossword = crossword;
            GridEntryFocusCallbackData *focus_data = malloc(sizeof(GridEntryFocusCallbackData));
            focus_data->x = j;
            focus_data->y = i;
            focus_data->orientation = orientation;
            focus_data->pos = last_pos;
            focus_data->crossword = crossword;
            focus_data->dictionary = dictionary;
            focus_data->tree_view = tree_view;
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_width_chars(GTK_ENTRY(entry), 2);
            gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
            gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
            gtk_grid_attach(GTK_GRID(grid), entry, j, i, 1, 1);
            gtk_widget_set_events(entry, GDK_BUTTON_PRESS_MASK);
            g_signal_connect(entry, "changed", G_CALLBACK(grid_entry_changed_callback), changed_data);
            g_signal_connect(entry, "button-press-event", G_CALLBACK(grid_entry_button_press_callback), focus_data);

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
    gtk_box_pack_start(GTK_BOX(hbox), sidebox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    return window;
}