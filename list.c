#include <stdlib.h>

#include "list.h"

List list_init()
{
    List L = malloc(sizeof(ListNode));
    if(L == NULL) return NULL;

    L->str = NULL;
    L->next = NULL;
    return L;
}

void list_add(List *L, char *str)
{
    ListNode *node = list_init();
    if(node == NULL) return;
    node->next == NULL;
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
    while(l->next != NULL)
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