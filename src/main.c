#include <stdio.h>

#include "template_editor.h"
#include "crossword_editor.h"
#include "dictionary.h"

#define DEFAULT_CROSSWORD_HEIGHT 15
#define DEFAULT_CROSSWORD_WIDTH 15

static void file_open_error(GtkWindow *parent)
{
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog = gtk_message_dialog_new(parent, flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
                                                "An error occured while opening the file.");

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static char* get_filename(GtkWidget *parent)
{
    GtkFileChooserNative *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_native_new("Open file", GTK_WINDOW(parent), action, "_Open", "_Cancel");
    int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(dialog);
        char* filename =  gtk_file_chooser_get_filename(file_chooser);
        g_object_unref(dialog);
        return filename;
    }
    else
    {
        g_object_unref(dialog);
        return NULL;
    }
}

static void show_window_callback(GtkWidget *window, gpointer data)
{
    gtk_widget_show_all(data);
}

static void template_new_callback(GtkWidget *button, gpointer data)
{
    Crossword *template = crossword_init(DEFAULT_CROSSWORD_WIDTH, DEFAULT_CROSSWORD_HEIGHT);
    GtkWidget *editor = template_editor_window_init(template, NULL);
    g_signal_connect(editor, "destroy", G_CALLBACK(show_window_callback), data);
    gtk_widget_hide(data);
    gtk_widget_show_all(editor);
}

static void template_open_callback(GtkWidget *button, gpointer data)
{
    char *filename = get_filename(data);
    if(filename == NULL) return;
    Crossword *template = crossword_load_from_file(filename);
    if(template == NULL)
    {
        file_open_error(data);
        return;
    }
    GtkWidget *editor = template_editor_window_init(template, filename);
    g_signal_connect(editor, "destroy", G_CALLBACK(show_window_callback), data);
    gtk_widget_hide(data);
    gtk_widget_show_all(editor);
}

static void crossword_new_callback(GtkWidget *button, gpointer data)
{
    char *filename = get_filename(data);
    if(filename == NULL) return;
    Crossword *template = crossword_load_from_file(filename);
    if(template == NULL)
    {
        file_open_error(data);
        return;
    }
    GtkWidget *editor = crossword_editor_window_init(template, NULL);
    g_signal_connect(editor, "destroy", G_CALLBACK(show_window_callback), data);
    gtk_widget_hide(data);
    gtk_widget_show_all(editor);
}

static void crossword_open_callback(GtkWidget *button, gpointer data)
{
    char *filename = get_filename(data);
    if(filename == NULL) return;
    Crossword *crossword = crossword_load_from_file(filename);
    if(crossword == NULL)
    {
        file_open_error(data);
        return;
    }
    GtkWidget *editor = crossword_editor_window_init(crossword, filename);
    g_signal_connect(editor, "destroy", G_CALLBACK(show_window_callback), data);
    gtk_widget_hide(data);
    gtk_widget_show_all(editor);
}

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
    g_signal_connect(template_new, "clicked", G_CALLBACK(template_new_callback), menu_window);

    GtkWidget *template_open = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(template_open), "Open a template");
    gtk_widget_set_margin_start(template_open, 10);
    gtk_widget_set_margin_end(template_open, 10);
    gtk_box_pack_start(GTK_BOX(box), template_open, TRUE, TRUE, 0);
    g_signal_connect(template_open, "clicked", G_CALLBACK(template_open_callback), menu_window);

    GtkWidget *crossword_new = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(crossword_new), "New crossword");
    gtk_widget_set_margin_start(crossword_new, 10);
    gtk_widget_set_margin_end(crossword_new, 10);
    gtk_box_pack_start(GTK_BOX(box), crossword_new, TRUE, TRUE, 0);
    g_signal_connect(crossword_new, "clicked", G_CALLBACK(crossword_new_callback), menu_window);

    GtkWidget *crossword_open = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(crossword_open), "Open a crossword");
    gtk_widget_set_margin_start(crossword_open, 10);
    gtk_widget_set_margin_end(crossword_open, 10);
    gtk_widget_set_margin_bottom(crossword_open, 10);
    g_signal_connect(crossword_open, "clicked", G_CALLBACK(crossword_open_callback), menu_window);

    gtk_box_pack_start(GTK_BOX(box), crossword_open, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(menu_window), box);

    g_signal_connect(menu_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(menu_window);

    gtk_main();
    return 0;
}