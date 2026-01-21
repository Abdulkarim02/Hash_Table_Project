#include<stdio.h>
#include <stdlib.h>
#include "hash_table.h"

ht_item* ht_item_(char *k, char *v){
    ht_item *item = (ht_item*) malloc(sizeof(ht_item));
    item->k=k;
    item->v=v;

    return item;
}
void ht_del_item(ht_item* item){
    free(item->k);
    free(item->v);
    free(item);
}

ht_hash_table* ht_hash_table_(int size){
    ht_hash_table* ht= (ht_hash_table*) malloc(sizeof(ht));
    ht->size=size;
    ht->count=0;
    ht->items= (ht_item**) calloc((size_t)ht->size,sizeof(ht_item*));

    return ht;
}
void ht_del_hash_table(ht_hash_table* ht){
    for(int i=0;i<ht->size;i++){
        if(ht->items[i] != NULL)
            ht_del_item(ht->items[i]);
    }
    free(ht->items);
    free(ht);
}

char* search(ht_hash_table *a, char *k){
    int key= hash(a,k);
    ht_item *item=a->items[key];
    if(item != NULL)
        return item->v;
    return NULL;
}

void insert(ht_hash_table *a, char *k, char *v){
    int key= hash(a,k);
    ht_item* item= ht_item_(k,v);
    if(a->items[key] == NULL){
        a->items[key]= item;
        a->count++;
    }
}

void deletes(ht_hash_table *a, char *k){
    int key= hash(a,k);
    if(a->items[key] != NULL){
        ht_del_item(a->items[key]);
        a->count--;
    }
}

int hash(ht_hash_table *a,char *k){
    int sum=0;
    char *c=k;
    while(*c){
        sum+= *c;
        c++;
    }
    int key= sum % a->size;
    return key;
}

void print_ht(ht_hash_table *a){
    for(int i=0;i<a->size;i++){
        if(a->items[i]!= NULL)
            printf("%d :(%s : %s)\n",i,a->items[i]->k,a->items[i]->v);
    }
}
int main(){
    ht_hash_table *ht = ht_hash_table_(53);
    char *a="mcderf",*b="qwerasd",*c="frecdcm";
    insert(ht,a,b);
    printf("%s\n",search(ht,a));
    print_ht(ht);
    ht_del_hash_table(ht);
}