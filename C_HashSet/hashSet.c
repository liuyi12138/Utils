#include "hashSet.h"

void htInit(hashSet *ht) {

    for (int i = 0; i < TABLE_SIZE; i++){
        ht[i] = (htItem*)malloc(sizeof(htItem));
        memset(ht[i], 0, sizeof(htItem));
    }
    return;
}


void insertItem(hashSet *ht, char* key, char* value) {

    unsigned int i = hash_33(key) % TABLE_SIZE;

    htItem *temp = ht[i];

    while (temp->next) {
        if(strcmp(key, temp->next->key) == 0) {
            temp->next->value = value;
            return;
        }
        temp = temp->next;
    }
    temp->next = (htItem*)malloc(sizeof(htItem));
    temp->next->key = key;
    temp->next->value = value;
    temp->next->next = NULL;
    return;

}


char* getValue(hashSet *ht, char* key) {

    unsigned int i = hash_33(key) % TABLE_SIZE;
    htItem *temp = ht[i];
    while(temp->next){
        if(strcmp(key, temp->next->key) == 0){
            return temp->next->value;
        }
        temp = temp->next;
    }
    return "";

}


void deleteItem(hashSet *ht, char* key) {

    unsigned int i = hash_33(key) % TABLE_SIZE;
    htItem *temp = ht[i];
    while(temp->next){
        if(strcmp(key, temp->next->key) == 0){
            htItem *tempNext = temp->next;
            temp->next = tempNext->next;
            free(tempNext);
            return;
        }
        temp = temp->next;
    }
    return;
}


void printHashSet(hashSet *ht) {

    for(int i = 0; i < TABLE_SIZE; ++i){
        htItem *temp = ht[i];
        while (temp->next) {
            printf("key: %s      value: %s\n",temp->next->key,temp->next->value);
            temp = temp->next;
        }
    }
    return;
}

int htLen(hashSet *ht) {
    int length = 0;
    for(int i = 0; i < TABLE_SIZE; ++i){
        htItem *temp = ht[i];
        while (temp->next) {
            length++;
            temp = temp->next;
        }
    }
    return length;

}






unsigned int hash_33(char* key) {
    int hash = 0;
    while (*key) {
        hash = 33*hash + *key++;
    }
    return hash;
}
