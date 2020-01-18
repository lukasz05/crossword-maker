#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list
{
    char *str;
    struct list *next;
} ListNode;

typedef ListNode *List; 

List* list_init();
void list_add(List L, char *str);
void list_clear(List L);
bool list_is_empty(List L);

#endif