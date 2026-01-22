#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#include "prime.c"

static int ht_get_hash(const char *s, int mod , int attempt);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);

static ht_item HT_DELETED_ITEM = {NULL,NULL};

static ht_item* ht_item_(char *k, char *v){
    ht_item *item = (ht_item*) malloc(sizeof(ht_item));
    item->k=k;
    item->v=v;

    return item;
}
static void ht_del_item(ht_item* item){
    // free(item->k);
    // free(item->v);
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
        if(ht->items[i] != NULL && ht->items[i]!= &HT_DELETED_ITEM)
            ht_del_item(ht->items[i]);
    }
    free(ht->items);
    free(ht);
}

char* ht_search(ht_hash_table *a, char *k){
    int key = ht_get_hash(k,a->size,0);

    ht_item *item = a->items[key];
    int i=1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(item->k,k) == 0){
                return item->v;
            }
        }
        key= ht_get_hash(k,a->size,i);
        item = a->items[key];
        i++;
    }
    return NULL;
}

void ht_insert(ht_hash_table *a, char *k, char *v){
    const int load = a->count * 100 / a->size;
    if (load > 70) {
        ht_resize_up(a);
    }
    ht_item *item= ht_item_(k,v);

    int key,attempt =0;
    int first_deleted_index = -1;
    key= ht_get_hash(k,a->size,attempt);

    
    ht_item *curr_item = a->items[key];
    while(curr_item != NULL){
        if(curr_item != &HT_DELETED_ITEM){// cheack if the key already exists
            if (strcmp(curr_item->k, k) == 0) {
                ht_del_item(curr_item);
                a->items[key] = item;
                return;
            }
        }
        else{
            if(first_deleted_index == -1)// get the first empty/deleted cell
                first_deleted_index= key;
        }
        attempt++;
        key= ht_get_hash(k,a->size,attempt);
        curr_item = a->items[key];
    }

    if (first_deleted_index != -1) {
        a->items[first_deleted_index] = item;
    } else {
        a->items[key] = item;
    }
    a->count++;
}



void ht_delete(ht_hash_table *a, char *k){
    const int load = a->count * 100 / a->size;
    if (load < 10) {
        ht_resize_down(a);
    }

    int key= ht_get_hash(k,a->size,0),i;

    ht_item *item = a->items[key];
    i=1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(item->k,k) == 0){
                ht_del_item(item);
                a->items[key]= &HT_DELETED_ITEM;
                a->count--;
            }
        }
        key= ht_get_hash(k,a->size,i);
        item = a->items[key];
        i++;
    }
    
}


static int ht_hash(const char *s, int prime, int mod){
    long hash=0;
    const int len = strlen(s);
    for(int i=0;i<len;i++){
        hash+= (pow(prime,len-(i+1))*s[i]);
        hash = hash % mod;
    }
    return (int)hash;
}
static int ht_get_hash(const char *s, int mod , int attempt){
    int hash_a,hash_b;
    hash_a= ht_hash(s,HT_PRIME1,mod);
    hash_b= ht_hash(s,HT_PRIME2,mod);
    return (hash_a + (attempt * (hash_b+1))) % mod;
}


static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = (ht_hash_table*) malloc(sizeof(ht_hash_table));
    ht->base_size = base_size;

    ht->size = next_prime(ht->base_size);

    ht->count = 0;
    ht->items = (ht_item**) calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->k, item->v);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}



static void print_ht(ht_hash_table *a){
    for(int i=0;i<a->size;i++){
        if(a->items[i]!= NULL && a->items[i] != &HT_DELETED_ITEM)
            printf("%d :(%s : %s)\n",i,a->items[i]->k,a->items[i]->v);
    }
}
int main(){
    ht_hash_table *ht = ht_new();
    char *a,*b,*c;
    a= (char *) malloc(2);
    b= (char *) malloc(7);
    c= (char *) malloc(6);

    a="gg"; b="qwerasd"; c="fredcm";
    ht_insert(ht,a,b);
    // ht_insert(ht,b,"!gg");
    // ht_delete(ht,b);
    
    printf("%s\n",ht_search(ht,a));
    printf("%s\n",ht_search(ht,b));
    // printf("%d\n",ht_hash(a, HT_PRIME1, 53));
    // print_ht(ht);
    ht_del_hash_table(ht);
}