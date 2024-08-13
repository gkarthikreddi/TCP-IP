#ifndef __SERIALIZE__
#define __SERIALIZE__

typedef struct ser_buff{
    #define SERIALIZE_BUFFER_DEFAULT_SIZE 512
    void *b;
    int size;
    int next;
    int checkpoint;
} ser_buff;

void init_serialized_buffer(ser_buff **b, int size);
void serialize_string(ser_buff *b, char *data, int nbytes);
int  get_serialize_buffer_size(ser_buff *b);
void reset_serialize_buffer(ser_buff *b);

#endif
