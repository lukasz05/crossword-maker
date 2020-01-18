#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_add(List *L, char *str)
{
    ListNode *node = malloc(sizeof(ListNode));
    if(node == NULL) return;
    node->next = NULL;
    node->str = malloc(sizeof(char) * (strlen(str) + 1));
    if(node->str == NULL)
    {
        free(node);
        return;
    }
    strcpy(node->str, str);

    if(*L != NULL)
    {
        ListNode *l = *L;
        while(l->next != NULL)
            l = l->next;
        l->next = node;
    }
    else 
        *L = node;
}

void list_clear(List *L)
{
    if(L == NULL) return;
    ListNode *l = *L;
    while(l != NULL)
    {
        ListNode *tmp = l->next;
        free(l);
        l = tmp;
    }
    *L = NULL;
}

bool list_is_empty(List L)
{
    return (L == NULL);
}

int list_size(List L)
{
    if(list_is_empty(L)) return 0;

    int size = 0;
    ListNode *l = L;
    while(l != NULL)
    {
        size++;
        l = l->next;
    }
    return size;
}