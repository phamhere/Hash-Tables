#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *));

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // creating a new pair with the key and value arguments
  Pair *new_pair = create_pair(key, value);
  // making the index by using the hashing function on the key
  unsigned int index = hash(key, ht->capacity);
  // checking if the ht->storage[index] already exists
  if (ht->storage[index])
  {
    // if the stored key is different than the key argument, print a warning
    if (strcmp(ht->storage[index]->key, key) != 0)
    {
      fprintf(stderr, "%s\n", "You are overwriting a value with a different key");
    }
    // destroying old pair to make room for the new pair
    destroy_pair(ht->storage[index]);
  }
  // storing new pair into ht->storage
  ht->storage[index] = new_pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // getting storage index of key
  unsigned int index = hash(key, ht->capacity);
  // if the storage key matches the key argument at the index, destroy the pair there
  if (strcmp(ht->storage[index]->key, key) == 0)
  {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
  }
  // otherwise print an error
  else
  {
    fprintf(stderr, "%s\n", "This key does not exist");
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // retrieving storage index of key
  unsigned int index = hash(key, ht->capacity);
  // if index exists in storage, return the value there, otherwise return null
  if (ht->storage[index])
  {
    return ht->storage[index]->value;
  }
  else
  {
    return NULL;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // looping through ht->storage and destroying all pairs if they exist
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i])
    {
      destroy_pair(ht->storage[i]);
    }
  }
  // freeing ht->storage and ht
  free(ht->storage);
  free(ht);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
