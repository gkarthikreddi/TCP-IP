#include "cliconst.h"

typedef struct param_t param_t;
typedef struct ser_buff ser_buff;

typedef enum{
    ENABLE,
    DISABLE,
    NO_MODE
} op_mode;

typedef int (*cmd_callback)(param_t *param, ser_buff *tlv_buf, op_mode mode);
typedef int (*user_validation_callback)(char *leaf_value);

typedef enum {
    CMD,
    LEAF,
} param_type;

typedef struct cmd {
    char name[CMD_NAME_SIZE];
} cmd_t;

typedef enum{
    INT,
    STRING,
    IPV4,
    FLOAT,
    IPV6,
    BOOLEAN,
    INVALID,
    LEAF_MAX
} leaf_type;

typedef struct leaf {
    leaf_type type;
    char value[LEAF_VALUE_HOLDER_SIZE];
    user_validation_callback fn;
    char id[LEAF_ID_SIZE];
} leaf_t;

typedef union param_t_ {
    cmd_t *cmd;
    leaf_t *leaf;
} param_t_;

struct param_t {
    param_type type;
    param_t_ cmd_type;
    cmd_callback callback;
    char help[PARAM_HELP_STRING_SIZE];
    param_t *options[MAX_OPTION_SIZE];
    param_t *parent;
    int CMDCODE;
};

#define MIN(a,b)    (a < b ? a : b)

#define GET_PARAM_CMD(param)    (param->cmd_type.cmd)
#define GET_PARAM_LEAF(param)   (param->cmd_type.leaf)
#define GET_CMD_NAME(param)         (GET_PARAM_CMD(param)->name)
#define GET_LEAF_ID(param)          (GET_PARAM_LEAF(param)->id)
#define GET_PARAM_HELP_STRING(param) (param->help)
#define IS_PARAM_CMD(param)     (param->type == CMD)
#define IS_PARAM_LEAF(param)    (param->type == LEAF)
#define GET_LEAF_VALUE_PTR(param)   (GET_PARAM_LEAF(param)->value)

#define INVOKE_APPLICATION_CALLBACK_HANDLER(param, arg, mode)         \
                    param->callback(param, arg, mode);

#define IS_LEAF_VALIDATION_REGISTERED(param)                          \
                    (param->cmd_type.leaf->fn)
#define LEAF_VALIDATION(param, arg)                                   \
                    (param->cmd_type.leaf->fn(arg))
