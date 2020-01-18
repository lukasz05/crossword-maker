#include <stdlib.h>
#include <string.h>

#include "list.h"

List* list_init()
{
    List *list = malloc(sizeof(List));
    list->front = NULL;
    list->end = NULL;
    list->size = 0;
    return list;
}

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

    if(L->end != NULL)
    {
        L->end->next = node;
        L->end = node;
    }
    else 
    {
        L->front = node;
        L->end = node;
    }
    L->size++;
}

void list_clear(List *L)
{
    if(L->front == NULL) return;
    ListNode *l = L->front;
    while(l != NULL)
    {
        ListNode *tmp = l->next;
        free(l);
        l = tmp;
    }
    L->front = NULL;
    L->end = NULL;
}

bool list_is_empty(List L)
{
    return (L.size == 0);
}

int list_size(List L)
{
    return L.size;
}