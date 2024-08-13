#ifndef __LIBCLI__
#define __LIBCLI__

#include "cmd_hier.h"

void
init_param(param_t *param,              
           param_type type,
           char *cmd_name,              
           cmd_callback callback,
           user_validation_callback fn,
           leaf_type leaf_type,
           char *leaf_id,
           char *help);

void init_libcli();
void libcli_register_param(param_t *parent, param_t *child);
param_t * libcli_get_show_hook(void);
param_t * libcli_get_config_hook(void);
param_t * libcli_get_root_hook(void);
param_t * libcli_get_run_hook(void);
param_t * get_cmd_tree_cursor(void);
void set_param_cmd_code(param_t *param, int cmd_code);
void start_shell(void);

#endif

