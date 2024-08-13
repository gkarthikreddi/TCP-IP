#include "serialize.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>


void init_serialized_buffer(ser_buff **b, int size){
    (*b) = (ser_buff *)calloc(1, sizeof(ser_buff));
    (*b)->b = calloc(1, size);
    (*b)->size = size;
    (*b)->next = 0;
    (*b)->checkpoint = 0;
}

void serialize_string(ser_buff *b, char *data, int nbytes){
    if (b == NULL) assert(0);
    ser_buff *buff = (ser_buff *)(b);
    int available_size = buff->size - buff->next;
    char isResize = 0;

    while(available_size < nbytes){
        buff->size = buff->size * 2;
        available_size = buff->size - buff->next;
        isResize = 1;
    }

    if (isResize) buff->b = realloc(buff->b, buff->size); 

    memcpy((char *)buff->b + buff->next, data, nbytes);
    buff->next += nbytes;
    return;
}

int  get_serialize_buffer_size(ser_buff *b){
	return b->next;
}

void reset_serialize_buffer(ser_buff *b){
	b->next = 0;	
    b->checkpoint = 0;
}
