#include "cmdtlv.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static param_t root;
static param_t show;
static param_t config;
static param_t run;

ser_buff *tlv_buff;

extern void command_parser(void);

param_t * get_cmd_tree_cursor(void) {
    return &root;
}

void init_param(param_t *param,
        param_type type,
        char *cmd_name,
        cmd_callback callback,
        user_validation_callback fn,
        leaf_type leaf_type,
        char *leaf_id,
        char *help) {

    int i = 0;
    if(type == CMD){
        GET_PARAM_CMD(param) = (cmd_t *)calloc(1, sizeof(cmd_t));
        param->type = CMD;
        strncpy(GET_CMD_NAME(param), cmd_name, MIN(CMD_NAME_SIZE, strlen(cmd_name)));
        GET_CMD_NAME(param)[CMD_NAME_SIZE -1] = '\0';
    }

    if(type == LEAF){
        GET_PARAM_LEAF(param) = (leaf_t *)calloc(1, sizeof(leaf_t));
        param->type = LEAF;
        GET_PARAM_LEAF(param)->type = leaf_type;
        param->cmd_type.leaf->fn = fn;
        strncpy(GET_LEAF_ID(param), leaf_id, MIN(LEAF_ID_SIZE, strlen(leaf_id)));
        GET_LEAF_ID(param)[LEAF_ID_SIZE -1] = '\0';
    }
     
    param->parent = NULL;
    param->callback = callback;
    strncpy(GET_PARAM_HELP_STRING(param), help, MIN(PARAM_HELP_STRING_SIZE, strlen(help)));
    GET_PARAM_HELP_STRING(param)[PARAM_HELP_STRING_SIZE -1] = '\0';

    for(; i < MAX_OPTION_SIZE; i++){
        param->options[i] = NULL;
    }

    param->CMDCODE = -1;
}

void libcli_register_param(param_t *parent, param_t *child){
    if (!parent) parent = &root;

    for (int i = 0; i < CHILDREN_END_INDEX; i++) {
        if (parent->options[i])
            continue;
        
        parent->options[i] = child;
        child->parent = parent;
        return;
    }
    printf("Can't register child and parent\n");
    assert(0);
}

void init_libcli() {
    init_param(&root, CMD, "ROOT", 0, 0, INVALID, 0, "ROOT");

    init_serialized_buffer(&tlv_buff, TLV_MAX_BUFFER_SIZE);

    init_param(&show, CMD, "show", 0, 0, INVALID, 0, "show");
    libcli_register_param(&root, &show);

    init_param(&config, CMD, "config", 0, 0, INVALID, 0, "config");
    libcli_register_param(&root, &config);

    init_param(&run, CMD, "run", 0, 0, INVALID, 0, "run");
    libcli_register_param(&root, &run);
}

void start_shell() {
    command_parser();
}

param_t * libcli_get_show_hook(void) {
    return &show;
}

param_t * libcli_get_config_hook(void) {
    return &config;
}

param_t * libcli_get_root_hook(void) {
    return &root;
}

param_t * libcli_get_run_hook(void) {
    return &run;
}

void set_param_cmd_code(param_t *param, int cmd_code) {
    param->CMDCODE = cmd_code;
}
