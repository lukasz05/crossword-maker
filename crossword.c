#include <stdlib.h>

#include "crossword.h"

Crossword* crossword_init(int width, int height)
{
    Crossword *crossword = (Crossword*)malloc(sizeof(Crossword));
    if(crossword == NULL)
        return NULL;

    crossword->width = width;
    crossword->height = height;
    crossword->content = malloc(height * sizeof(int*));
    if(crossword->content == NULL)
        return NULL;

    for(int i = 0; i < height; i++)
    {
        crossword->content[i] = malloc(width * sizeof(int));
        if(crossword->content[i] == NULL)
        {
            free(crossword->content);
            return NULL;
        }

        for(int j = 0; j < width; j++)
            crossword->content[i][j] = ' ';
    }
    return crossword;
}

void crossword_destroy(Crossword *crossword)
{
    if(crossword == NULL) return;
    for(int i = 0; i < crossword->height; i++)
        free(crossword->content[i]);
    free(crossword->content);
    free(crossword);
}

void crossword_clear(Crossword *crossword)
{
    for(int i = 0; i < crossword->height; i++)
        for(int j = 0; j < crossword->width; j++)
            if(crossword->content[i][j] != 0)
                crossword->content[i][j] = ' ';
}

void crossword_invert_template(Crossword *crossword)
{
    for(int i = 0; i < crossword->height; i++)
    {
        for(int j = 0; j < crossword->width; j++)
        {
            if(crossword->content[i][j] == 0)
                crossword->content[i][j] = ' ';
            else
                crossword->content[i][j] = 0;
        }
    }
}

void crossword_set_template_white(Crossword *crossword)
{
    for(int i = 0; i < crossword->height; i++)
        for(int j = 0; j < crossword->width; j++)
            crossword->content[i][j] = ' ';
}

void crossword_set_template_black(Crossword *crossword)
{
    for(int i = 0; i < crossword->height; i++)
        for(int j = 0; j < crossword->width; j++)
            crossword->content[i][j] = 0;
}