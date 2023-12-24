#ifndef DICT_H
#define DICT_H

/**
 * @brief Represents a dictionary.
 */
typedef struct Dict_t Dict;

/**
 * @brief Creates a new dictionary.
 *
 * @param m size of the hash table.
 *
 * @return The created dictionary.
 */
Dict *dictCreate(int m);

/**
 * @brief Frees a dictionary.
 *
 * @param d Dictionary to free.
 */
void dictFree(Dict *d);

/**
 * @brief Search for a given key in a dictionary
 * and return the associated value.
 *
 * @param d Dictionary to search in.
 * @param key Key to search for.
 *
 * @returns the double value if the key was found, or exit the program if the key
 * was not found.
 */
double dictSearch(Dict *d, char *key);

/**
 * @brief Search for a given key in a dictionary
 * and return 1 if the key is found, 0 otherwise.
 *
 * @param d Dictionary to search in.
 * @param key Key to search for.
 *
 * @returns 1 if the key was found, 0 otherwise.
 */
int dictContains(Dict *d, char *key);

/**
 * @brief Insert a new key-value pair in a dictionary.
 *
 * @param d Dictionary to insert in.
 * @param key Key to insert.
 * @param value Value to insert.
 */
void dictInsert(Dict *d, char *key, double value);

#endif
