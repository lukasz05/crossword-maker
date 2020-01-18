#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "list.h"

#define MAX_WORD_LENGTH 50

typedef struct dictionary
{
    List* words_by_length[MAX_WORD_LENGTH + 1];
} Dictionary;

Dictionary* dictionary_init();
Dictionary* dictionary_load_from_file(char *filename);
void dictionary_add_word(Dictionary* dict, char *word);
List* dictionary_find_words(Dictionary* dict, char *pattern);
void dictionary_clear(Dictionary* dict);

#endif