#ifndef CROSSWORD_EDITOR_H
#define CROSSWORD_EDITOR_H

#include <gtk/gtk.h>

#include "crossword.h"

GtkWidget* crossword_editor_window_init(Crossword *crossword, char *filename);

#endif