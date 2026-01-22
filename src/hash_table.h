#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include<stdio.h>

const int HT_PRIME1 = 151;
const int HT_PRIME2 = 163;
const int HT_INITIAL_BASE_SIZE =50;



typedef struct
{
    char *k;
    char *v;
} ht_item;

typedef struct
{
    int size;
    int base_size;
    int count;
    ht_item **items;

} ht_hash_table;


ht_hash_table* ht_hash_table_(int size);
void ht_del_hash_table(ht_hash_table* ht);
ht_hash_table* ht_new();

char* ht_search(ht_hash_table *a, char *k);

void ht_insert(ht_hash_table *a, char *k, char *v);

void ht_delete(ht_hash_table *a, char *k);




#endif