#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in the existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // making a new LinkedPair from the key and value arguments
  LinkedPair *new_pair = create_pair(key, value);
  // getting the index for storage from the hash function on the key argument
  unsigned int index = hash(key, ht->capacity);
  // making a current and last pair to loop through LinkedPair chain
  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair;
  // unless there is something at the index, assign the storage index with the new pair
  if (ht->storage[index])
  {
    // keep looping while current_pair isn't null
    while (current_pair)
    {
      // if the key at current_pair key is the same as the key arg, overwrite the value and return
      if (strcmp(current_pair->key, key) == 0)
      {
        destroy_pair(current_pair);
        current_pair = new_pair;
        return;
      }
      // if the key at current_pair key is different from the key arg, move onto next in LinkedPair chain
      else
      {
        last_pair = current_pair;
        current_pair = last_pair->next;
      }
    }
    // if no key matches were found in LinkedPair chain, add onto LinkedPair chain
    current_pair = new_pair;
  }
  else
  {
    ht->storage[index] = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  // getting the index for storage from the hash function on the key argument
  unsigned int index = hash(key, ht->capacity);
  // initializing current_pair and last_pair to use in loop to track where in a LinkedPair chain we are
  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair;
  // looping while current_pair isn't null
  while (current_pair)
  {
    // if the key at current_pair key is the same as the key argument
    if (strcmp(current_pair->key, key) == 0)
    {
      // if the current_pair has another value after it
      if (current_pair->next != NULL)
      {
        // if the current_pair is the head of the LinkedPair chain, make the head its next value
        if (current_pair == ht->storage[index])
        {
          ht->storage[index] = current_pair->next;
        }
        // if the current_pair is in the middle of a LinkedPair chain, link up the previous and next chain
        else
        {
          last_pair->next = current_pair->next;
        }
      }
      // freeing the space of the current_pair and setting the pointer to null
      destroy_pair(current_pair);
      current_pair = NULL;
    }
    // if the key at current_pair is different from the key argument, move onto next in LinkedPair chain
    else
    {
      last_pair = current_pair;
      current_pair = last_pair->next;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // getting the index for storage from the hash function on the key argument
  unsigned int index = hash(key, ht->capacity);
  // looping while ht->storage[index] isn't null
  while (ht->storage[index])
  {
    // if the storage index key matches the key arg, return the value
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      return ht->storage[index]->value;
    }
    // otherwise move onto next in LinkedPair
    else
    {
      ht->storage[index] = ht->storage[index]->next;
    }
  }
  // return null if nothing is found
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  // looping through entire storage
  for (int i = 0; i < ht->capacity; i++)
  {
    // if storage has a value at index i and loops while ht->storage[i] isn't null
    while (ht->storage[i])
    {
      // free up the pair, pair key, and pair value
      destroy_pair(ht->storage[i]);
      // moves onto next in LinkedPair chain
      ht->storage[i] = ht->storage[i]->next;
    }
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  // allocating space for new_ht and its storage while assigning its capacity
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity * 2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));
  // looping through ht->storage and copying values into new_ht->storage
  for (int i = 0; i < ht->capacity; i++)
  {
    // if there is a value in ht->storage
    if (ht->storage[i])
    {
      new_ht->storage[i] = ht->storage[i];
      // if there is a LinkedPair chain, go down chain copying and values to new_ht
      while (ht->storage[i]->next)
      {
        new_ht->storage[i]->next = ht->storage[i]->next;
        ht->storage[i] = ht->storage[i]->next;
        new_ht->storage[i] = new_ht->storage[i]->next;
      }
    }
  }

  destroy_hash_table(ht);

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
