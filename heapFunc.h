#include <stdlib.h>
#include <assert.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#ifndef test
#define test
#include "Ex4.h"
#endif



typedef struct data_block
{
    size_t size;
    struct data_block *next;
    int free;
} data_block;

data_block *start_blocks;
data_block *last_block;



data_block *find_free_block(size_t size)
{
    data_block *cur = start_blocks;
    while (cur && !(cur->free && cur->size >= size))
    {
        cur = cur->next;
    }
    return cur;
}

data_block* get_new_space(size_t size)
{
    data_block *new_block =(data_block*) sbrk(0);
    void *request = sbrk(size + sizeof(data_block));
    if (request==(void*)-1)
    {
        return NULL ;
    }
    if(last_block!=NULL){
        last_block->next=new_block;
    }
    last_block=new_block;
    new_block->free=0;
    new_block->size=size;
    new_block->next=NULL;

    return new_block;
}

void* my_malloc(size_t size){
    pthread_mutex_lock(&lock2);
    if(size<=0){
        return NULL;
    }
    data_block *new_block;
    if(start_blocks==NULL){
        new_block=(data_block*)get_new_space(size);
        if(new_block==NULL){
            return NULL;
        }
        start_blocks=new_block;
        last_block=new_block;
    }
    else{
        new_block=find_free_block(size);
        if(new_block==NULL){
            new_block=get_new_space(size);
            if(new_block==NULL){
                return NULL;
            }
        }
        new_block->free=0;
    }
    pthread_mutex_unlock(&lock2);
    return (new_block+1);
}

void my_free(void *p_tofree){
    pthread_mutex_lock(&lock2);
    if(p_tofree==NULL){
        return ;
    }
    data_block *block_p=((data_block*)p_tofree)-1;
    assert(block_p->free==0);
    block_p->free=1;  //set the block to free
    pthread_mutex_unlock(&lock2);
}

void *my_calloc(size_t nitems, size_t size){
    void* pt= my_malloc(nitems*size);
    memset(pt,0,size);
    return pt;
}

// int main(){
//     int *arr=(int*)my_malloc(10*sizeof(int));
//     for(int i=0;i<10;i++){
//         arr[i]=i;
//         printf("%d,",arr[i]);
//     }
//     free(arr);
// }