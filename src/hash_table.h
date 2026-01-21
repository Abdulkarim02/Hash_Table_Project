#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include<stdio.h>

typedef struct
{
    char *k;
    char *v;
} ht_item;

typedef struct
{
    int size;
    int count;
    ht_item **items;

} ht_hash_table;

ht_item* ht_item_(char *k, char *v);
void ht_del_item(ht_item* item);

ht_hash_table* ht_hash_table_(int size);
void ht_del_hash_table(ht_hash_table* ht);

char* search(ht_hash_table *a, char *k);

void insert(ht_hash_table *a, char *k, char *v);

void deletes(ht_hash_table *a, char *k);

int hash(ht_hash_table *a,char *k);




#endif