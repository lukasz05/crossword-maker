#include <stdlib.h>
#include <stdio.h>

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

void crossword_resize(Crossword *crossword, int width, int height)
{

    if(crossword->height < height) 
        crossword->content = realloc(crossword->content, height * sizeof(int*));

    int H = (crossword->height > height) ? crossword->height : height;
    for(int i = 0; i < H; i++)
    {   
        if(i >= crossword->height)
        {
            crossword->content[i] = malloc(width * sizeof(int));
            for(int j = 0; j < width; j++)
                crossword->content[i][j] = ' ';
        }
        else if(i >= height)
            free(crossword->content[i]);
        else
        {
            crossword->content[i] = realloc(crossword->content[i], width * sizeof(int));
            if(crossword->width < width)
                for(int j = crossword->width; j < width; j++)
                    crossword->content[i][j] = ' ';
        }
    }

    if(crossword->height > height)
        crossword->content = realloc(crossword->content, height * sizeof(int*));

    crossword->height = height;
    crossword->width = width;
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

bool crossword_save_to_file(Crossword *crossword, char *filename)
{
    FILE *file = fopen(filename, "w");
    if(file == NULL) return false;

    fprintf(file, "%d %d\n", crossword->width, crossword->height);
    for(int i = 0; i < crossword->height; i++)
    {
        for(int j = 0; j < crossword->width; j++)
            fprintf(file, "%d ", crossword->content[i][j]);
        fprintf(file, "\n");
    }
    fclose(file);
    return true;
}

Crossword* crossword_load_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL) return NULL;

    int width, height;
    fscanf(file, "%d %d", &width, &height);
    Crossword *crossword = crossword_init(width, height);
    if(crossword == NULL)
    {
        fclose(file);
        return NULL;
    }
    crossword->width = width;
    crossword->height = height;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(fscanf(file, "%d", &(crossword->content[i][j])) != 1)
            {
                fclose(file);
                free(crossword);
                return NULL;
            }
        }
    }

    fclose(file);
    return crossword;
}

char* crossword_get_word_pattern(Crossword *crossword, int x, int y, int orientation)
{
    int length = 0;
    if(x < 0 || x >= crossword->width) return NULL;
    if(y < 0 || y >= crossword->height) return NULL;
    int i = y;
    int j = x;
    while(i < crossword->height && j < crossword->width && crossword->content[i][j] != 0)
    {
        length++;
        if(orientation == 0) j++;
        else i++;
    }
    if(length == 0) return NULL;

    char *pattern = malloc(sizeof(char) * length + 1);
    if(pattern == NULL) return NULL;

    int pos = 0;
    while(y < crossword->height && x < crossword->width && crossword->content[y][x] != 0)
    {
        char c = crossword->content[y][x];
        if(c == ' ') pattern[pos] = '*';
        else pattern[pos] = c;
        pos++;
        if(orientation == 0) x++;
        else y++;
    }
    pattern[length] = '\0';
    return pattern;
}