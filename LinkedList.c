#include <stdlib.h>
#include <stdio.h>

#include "LinkedList.h"

struct Node_t
{
    void *data;
    Node *next;
};

struct List_t
{
    Node *head;
    Node *tail;
    size_t length;
};

Node *llCreateNode(void *data)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
        return NULL;

    n->data = data;
    n->next = NULL;

    return n;
}

void *llData(Node *node)
{
    return node->data;
}

Node *llNext(Node *node)
{
    return node->next;
}

List *llCreateEmpty()
{
    List *list = malloc(sizeof(List));
    if (!list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;
}

Node *llHead(const List *list)
{
    return list->head;
}

Node *llTail(const List *list)
{
    return list->tail;
}

size_t llLength(const List *list)
{
    return list->length;
}

void llFree(List *list)
{
    Node *n = list->head;
    Node *nn = NULL;

    while (n)
    {
        nn = n->next;
        free(n);
        n = nn;
    }

    free(list);
}

void llFreeData(List *list)
{
    Node *n = list->head;
    Node *nn = NULL;

    while (n)
    {
        nn = n->next;
        free(n->data);
        free(n);
        n = nn;
    }

    free(list);
}

void llInsertFirst(List *list, Node *node)
{
    if (!list->head)
        list->tail = node;

    node->next = list->head;
    list->head = node;
    list->length++;
}

Node *llPopFirst(List *list)
{
    if (!list->head)
        return NULL;

    Node *node = list->head;

    list->length--;
    list->head = node->next;
    node->next = NULL;

    if (!list->head)
        list->tail = NULL;

    return node;
}

void llInsertLast(List *list, Node *node)
{
    node->next = NULL;

    if (list->head)
    {
        list->tail->next = node;
        list->tail = node;
        list->length++;
    }
    else
        llInsertFirst(list, node);
}

int llFilter(List *list, int (*test)(void *, void *),
             void *params, int dryRun, int freeData)
{

    int count = 0;
    Node *pn = NULL;
    Node *cn = list->head;

    while (cn)
    {

        Node *nn = cn->next;

        if (test(cn->data, params))
        {
            count++;
            if (!dryRun)
            {
                if (pn)
                    pn->next = nn;
                else
                    list->head = nn;
                if (freeData)
                    free(cn->data);
                free(cn);
                list->length--;
            }
            cn = nn;
        }
        else
        {
            pn = cn;
            cn = nn;
        }
    }

    return count;
}
