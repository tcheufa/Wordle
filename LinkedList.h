#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>

/**
 * @brief Represents a node of a linked list.
 */
typedef struct Node_t Node;

/**
 * @brief Represents a linked list.
 */
typedef struct List_t List;

/**
 * @brief Creates a new node.
 *
 * @param data Data to store in the node.
 *
 * @return The created node.
 */
Node *llCreateNode(void *data);

/**
 * @brief Get the data stored in a node.
 *
 * @param node Node to get the data from.
 *
 * @return The data stored in the node.
 */
void *llData(Node *node);

/**
 * @brief Get the next node of a node.
 *
 * @param node Node to get the next node from.
 *
 * @return The next node.
 */
Node *llNext(Node *node);

/**
 * @brief Creates a new empty list.
 *
 * @return The created list.
 */
List *llCreateEmpty(void);

/**
 * @brief Get the first node of a list.
 *
 * @param list List to get the first node from.
 *
 * @return the first node of the list.
 */
Node *llHead(const List *list);

/**
 * @brief Get the last node of a list.
 *
 * @param list List to get the last node from.
 *
 * @return the last node of the list.
 */
Node *llTail(const List *list);

/**
 * @brief Get the length of a list.
 *
 * @param list List to get the length from.
 *
 * @return the length of the list.
 */
size_t llLength(const List *list);

/**
 * @brief Free a list, but not the data stored in the nodes.
 *
 * @param list List to free.
 */
void llFree(List *list);

/**
 * @brief Free a list, and the data stored in the nodes.
 *
 * @param list List to free.
 */
void llFreeData(List *list);

/**
 * @brief Add a node at the beginning of a list.
 *
 * @param list List to add the node to.
 * @param node Node to add.
 */
void llInsertFirst(List *list, Node *node);

/**
 * @brief the node at the start of a list and returns a pointer
 * to the removed node.
 *
 * @param list List to remove the node from.
 *
 * @return The removed node.
 */
Node *llPopFirst(List *list);

/**
 * @brief Add a node at the end of a list.
 *
 * @param list List to add the node to.
 * @param node Node to add.
 */
void llInsertLast(List *list, Node *node);

/**
 * @brief Remove all nodes in the list that pass some test. Return the number
 * of nodes removed. If dryRun is true, no nodes are actually removed but
 * the function still returns the number of nodes to be removed. If a node
 * is removed, its data is freeed only if freeData is true.
 *
 * @param list List to remove the nodes from.
 * @param test Function to test if a node should be removed.
 * @param params Parameters to pass to the test function.
 * @param dryRun If true, no nodes are actually removed.
 * @param freeData If true, the data of the removed nodes is freed.
 *
 * @return The number of nodes removed.
 */
int llFilter(List *list, int (*test)(void *, void *),
			 void *params, int dryRun, int freeData);

#endif // !_LINKED_LIST_H_
