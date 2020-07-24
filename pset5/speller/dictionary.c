// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dictionary.h"
#include <strings.h>
#include <string.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *create(node *list, const char *word);
node *insert(node *list, const char *word);
bool find(node *list, const char *word);
void destroy(node *list);

// Number of buckets in hash table
//const unsigned int N = 4294967295;
const unsigned int N = 175000;

// Number of words in dictionary
unsigned int w = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // convert word to lowercase
    int n = strlen(word);
    char lword[n + 1];
    for (int i = 0; i < n; i++)
    {
        lword[i] = tolower(word[i]);
    }
    lword[n] = '\0';

    // check for word in dictionary
    unsigned int h = hash(lword);
    if (find(table[h], lword))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// djb2
// http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    // ensure file can be opened
    if (file == NULL)
    {
        return false;
    }

    unsigned int h;
    char word[sizeof(char) * (LENGTH + 1)];
    // scan through dictionary one line at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        h = hash(word);
        // create node if empty
        if (table[h] == NULL)
        {
            table[h] = create(table[h], word);
        }
        // insert node if already created
        else
        {
            table[h] = insert(table[h], word);
        }
        w++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // words counted during dictionary load
    if (w == 0)
    {
        return 0;
    }
    else
    {
        return w;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            destroy(table[i]);
        }
    }
    return true;
}

node *create(node *list, const char *word)
{
    // allocate memory for new node
    node *n = malloc(sizeof(node));
    // populate node
    if (n != NULL)
    {
        strcpy(n->word, word);
        n->next = NULL;
    }
    list = n;
    return list;
}

node *insert(node *list, const char *word)
{
    // allocate memory for new node
    node *n = malloc(sizeof(node));
    // populate node
    if (n != NULL)
    {
        strcpy(n->word, word);
        n->next = NULL;
    }
    // find end of list and insert
    node *tmp = list;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    n->next = list;
    list = n;
    return list;
}

bool find(node *list, const char *word)
{
    // node doesn't exist
    if (list == NULL)
    {
        return false;
    }
    // check for word through linked list
    node *trav = list;
    while (trav->next != NULL)
    {
        if (strcasecmp(trav->word, word) == 0)
        {
            return true;
        }
        else
        {
            trav = trav->next;
        }
    }
    if (strcasecmp(trav->word, word) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void destroy(node *list)
{
    // base case
    if (list->next == NULL)
    {
        free(list);
        return;
    }
    // next node
    node *tmp = list;
    list = list->next;
    free(tmp);
    // recursively call destroy
    return destroy(list);
}