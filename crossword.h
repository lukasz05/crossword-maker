#ifndef CROSSWORD_H
#define CROSSWORD_H

typedef struct _Crossword 
{
    int width;
    int height;
    int **content;
} Crossword;

Crossword* crossword_init(int width, int height);
void crossword_destroy(Crossword *crossword);
void crossword_clear(Crossword *crossword);
void crossword_invert_template(Crossword *crossword);
void crossword_set_template_white(Crossword *crossword);
void crossword_set_template_black(Crossword *crossword);
int crossword_save_to_file(char *filename);
Crossword* crossword_load_from_file(char *filename);

#endif