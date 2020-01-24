#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>

#include "dictionary.h"

#define BUF_SIZE 100

Dictionary* dictionary_init()
{
    Dictionary *dict = malloc(sizeof(Dictionary));
    if(dict == NULL) return NULL;
    for(int i = 0; i <= MAX_WORD_LENGTH; i++)
        dict->words_by_length[i] = list_init();
    return dict;
}

Dictionary* dictionary_load_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL) return NULL;

    Dictionary *dict = dictionary_init();
    if(dict == NULL)
    {
        fclose(file);
        return NULL;
    }

    char buf[BUF_SIZE];
    while(fgets(buf, BUF_SIZE, file) != NULL)
    {
        int len = strlen(buf);
        if(buf[len - 1] != '\n') continue;
        for(int i = len - 1; i >= 0; i--)
        {
            if(isspace(buf[i]))
            {
                buf[i] = '\0';
                len--;
            }
            else break;
        }
        int utf8_len = g_utf8_strlen(buf, -1);
        if(utf8_len > MAX_WORD_LENGTH) continue;
        char *tmp = g_utf8_strup(buf, len);
        list_add(dict->words_by_length[utf8_len], tmp);
        free(tmp);
    }
    fclose(file);
    return dict;
}

void dictionary_add_word(Dictionary* dict, char *word)
{
    int len = g_utf8_strlen(word, -1);
    if(len > MAX_WORD_LENGTH) return;
    list_add(dict->words_by_length[len], word);
}

List* dictionary_find_words(Dictionary* dict, char *pattern)
{
    int len = g_utf8_strlen(pattern, -1);
    if(len > MAX_WORD_LENGTH) NULL;

    List *result = list_init();
    ListNode *L = dict->words_by_length[len]->front;
    while(L != NULL)
    {
        bool ok = false;
        char *a = pattern;
        char *b = L->str;
        if(strlen(a) == strlen(b))
        {
            ok = true;
            while(*a != '\0' && *b != '\0')
            {
                if(*a != '*' && g_utf8_get_char(a) != g_utf8_get_char(b))
                {
                    ok = false;
                    break;
                }
                a = g_utf8_next_char(a);
                b = g_utf8_next_char(b);
            }
        }
        if(ok) list_add(result, L->str);
        L = L->next;
    }
    return result;
}

void dictionary_clear(Dictionary* dict)
{
    for(int i = 0; i <= MAX_WORD_LENGTH; i++)
        list_clear(dict->words_by_length[i]);
}