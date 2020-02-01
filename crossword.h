#ifndef CROSSWORD_H
#define CROSSWORD_H

#include <stdbool.h>

#define MAX_CROSSWORD_WIDTH 25
#define MAX_CROSSWORD_HEIGHT 25

typedef struct _Crossword 
{
    int width;
    int height;
    int **content;
} Crossword;

Crossword* crossword_init(int width, int height);
void crossword_destroy(Crossword *crossword);
void crossword_resize(Crossword *crossword, int width, int height);
void crossword_clear(Crossword *crossword);
void crossword_invert_template(Crossword *crossword);
void crossword_set_template_white(Crossword *crossword);
void crossword_set_template_black(Crossword *crossword);
bool crossword_save_to_file(Crossword *crossword, char *filename);
Crossword* crossword_load_from_file(char *filename);
char* crossword_get_word_pattern(Crossword *crossword, int x, int y, int orientation);
void crossword_place_word(Crossword *crossword, int x, int y, int orientation, char *word);

#endif