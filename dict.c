#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "dict.h"

typedef struct Node_t
{
    char *key;
    double value;
    struct Node_t *next;
} Node;

struct Dict_t
{
    Node **array;
    int arraySize;
};

static unsigned int h(Dict *d, char *key);
static void terminate(char *m);

static unsigned int h(Dict *d, char *key)
{
    unsigned long hash = 5381;
    while (*key != '\0')
    {
        hash = hash * 33 + *key;
        key++;
    }
    return hash % d->arraySize;
}

static void terminate(char *m)
{
    printf("%s\n", m);
    exit(EXIT_FAILURE);
}

Dict *dictCreate(int m)
{
    Dict *d = malloc(sizeof(Dict));
    if (d == NULL)
        terminate("Dict cannot be created");

    d->array = calloc(m, sizeof(Node *));
    if (d->array == NULL)
        terminate("Dict cannot be created");

    d->arraySize = m;
    return d;
}

void dictFree(Dict *d)
{
    Node *n, *nn;
    for (int i = 0; i < d->arraySize; i++)
    {
        n = d->array[i];
        while (n != NULL)
        {
            nn = n->next;
            free(n->key);
            free(n);
            n = nn;
        }
    }

    free(d->array);
    free(d);
}

double dictSearch(Dict *d, char *key)
{
    Node *p = d->array[h(d, key)];
    while (p != NULL && strcmp(p->key, key) != 0)
        p = p->next;

    if (p != NULL)
        return p->value;

    else
    {
        fprintf(stderr, "dictSearch: Key %s not found.\n", key);
        exit(EXIT_FAILURE);
    }
}

int dictContains(Dict *d, char *key)
{
    Node *p = d->array[h(d, key)];
    while (p != NULL && strcmp(p->key, key) != 0)
        p = p->next;

    return (p != NULL);
}

void dictInsert(Dict *d, char *key, double value)
{
    int hashval = h(d, key);
    Node *p = d->array[hashval];
    while (p != NULL && strcmp(p->key, key) != 0)
        p = p->next;

    if (p != NULL)
        p->value = value;

    else
    {
        Node *newNode = malloc(sizeof(Node));
        if (!newNode)
            terminate("New node cannot be created.");

        int len = strlen(key);
        char *k = malloc((len + 1) * sizeof(char));
        if (!k)
            terminate("New node cannot be created.");

        strcpy(k, key);

        newNode->key = k;
        newNode->value = value;
        newNode->next = d->array[hashval];
        d->array[hashval] = newNode;
    }
}
