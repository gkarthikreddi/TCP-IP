#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmdtlv.h"

extern ser_buff *tlv_buff;
static tlv_struct tlv;
static tlv_struct command_code_tlv;

param_t* find_matching_param(param_t** options, char* name) {

    int idx = -1;
    for (int i = 0; i < CHILDREN_END_INDEX; i++) {
        if (options[i] == NULL) break;


        if (IS_PARAM_LEAF(options[i])) {
            idx = i;
            continue;
        }

        if (!strcmp(options[i]->cmd_type.cmd->name, name))
            return options[i];
    }

    if (idx >= 0) return options[idx];
    return NULL;
}

void build_tlv_buffer(param_t *param, char *token) {
    memset(&tlv, 0, sizeof(tlv_struct));
    prepare_tlv_from_leaf(GET_PARAM_LEAF(param), (&tlv));
    put_value_in_tlv((&tlv), token);
    strncpy(GET_LEAF_VALUE_PTR(param), token, MIN(strlen(token), LEAF_VALUE_HOLDER_SIZE));
    GET_LEAF_VALUE_PTR(param)[strlen(token)] = '\0';
    collect_tlv(tlv_buff, &tlv);
}

void parse(char **tokens, int idx) {
    param_t *param = get_cmd_tree_cursor();
    param_t *parent = param;
    op_mode mode = NO_MODE;

    for (int i = 0; i < idx; i++) {
        if (!strcmp(tokens[i], "no")) {
            mode = DISABLE;
            continue;
        }

        param = find_matching_param(&parent->options[0], tokens[i]);
        if (!param) break;
        
        if (IS_PARAM_LEAF(param)) {
            if (IS_LEAF_VALIDATION_REGISTERED(param)) {
                if (!LEAF_VALIDATION(param, tokens[i])) {
                    printf("Given <param-value> is not suitable for: %s", param->cmd_type.leaf->id);
                    break;
                }
            }
            build_tlv_buffer(param, tokens[i]);
        }

        if (!strcmp(GET_CMD_NAME(param), "config")) mode = ENABLE;
        parent = param;
    }

    if (parent) {
        memset(command_code_tlv.value, 0, LEAF_VALUE_HOLDER_SIZE);
        sprintf(command_code_tlv.value, "%d", parent->CMDCODE);
        collect_tlv(tlv_buff, &command_code_tlv);
    }

    if (parent->callback)
        INVOKE_APPLICATION_CALLBACK_HANDLER(parent, tlv_buff, mode);
}

void command_parser(void) {
    while(1) {
        char str[100];
        char *tokens[30];
        int idx = 0;
        
        printf("tcp-ip> ");
        fgets(str, sizeof(str), stdin);
        int len = strlen(str);
        str[len-1] = '\0';

        char *token = strtok(str, " ");
        while (token != NULL) {
            tokens[idx++] = token;
            token = strtok(NULL, " ");
        }

        parse(tokens, idx);
        reset_serialize_buffer(tlv_buff);
        printf("\n");
    }
}
