#ifndef TEMPLATE_EDITOR_H
#define TEMPLATE_EDITOR_H

#include <gtk/gtk.h>

#include "crossword.h"

#define MAX_CROSSWORD_WIDTH 20
#define MAX_CROSSWORD_HEIGHT 20

GtkWidget* template_editor_window_init(Crossword *template);

#endif