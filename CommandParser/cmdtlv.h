#include "libcli.h"
#include "serialize.h"
#include <stdlib.h>
#include <assert.h>

#ifndef __CMDTLV__H
#define __CMDTLV__H

#pragma pack (push,1)
typedef struct tlv_struct{
    leaf_type leaf_type;
    char leaf_id[LEAF_ID_SIZE];
    char value[LEAF_VALUE_HOLDER_SIZE];
} tlv_struct;
#pragma pack(pop)

#define prepare_tlv_from_leaf(leaf, tlvptr)    \
    tlvptr->leaf_type = leaf->type;       \
    strncpy(tlvptr->leaf_id, leaf->id, MIN(LEAF_ID_SIZE, strlen(leaf->id)));

#define put_value_in_tlv(tlvptr, _val)         \
    strncpy(tlvptr->value, _val, MIN(LEAF_VALUE_HOLDER_SIZE, strlen(_val)));

#define collect_tlv(ser_buff, tlvptr)           \
    serialize_string(ser_buff, (char *)tlvptr, sizeof(tlv_struct))

#define EXTRACT_CMD_CODE(ser_buff_ptr)  \
    atoi(((tlv_struct *)(ser_buff_ptr->b) + (get_serialize_buffer_size(ser_buff_ptr)/sizeof(tlv_struct) -1))->value)

#define TLV_LOOP_BEGIN(ser_buff, tlvptr)                                                \
{                                                                                       \
    assert(ser_buff);                                                                   \
    tlvptr = (tlv_struct *)(ser_buff->b);                                             \
    unsigned int i = 0, k = get_serialize_buffer_size(ser_buff)/sizeof(tlv_struct);   \
    for(; i < k-1; i++, tlvptr++)

#define TLV_LOOP_END    }

#endif
