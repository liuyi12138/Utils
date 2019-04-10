#ifndef HASHSET_H_INCLUDED
#define HASHSET_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 64

typedef struct htItem{
    char* key;
    char* value;
    struct htItem *next;
}htItem, *hashSet;

void htInit(hashSet *ht);

void insertItem(hashSet *ht, char* key, char* value);

char* getValue(hashSet *ht, char* key);

void deleteItem(hashSet *ht, char* key);

void printHashSet(hashSet *ht);

int htLen(hashSet *ht);

unsigned int hash_33(char* key);


#endif // HASHSET_H_INCLUDED
