#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list_node
{
    char *str;
    struct list_node *next;
} ListNode;

typedef struct list
{
    int size;
    ListNode *front;
    ListNode *end;
} List;

List *list_init();
void list_add(List *L, char *str);
void list_clear(List *L);
bool list_is_empty(List L);
int list_size(List L);

#endif