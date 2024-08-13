#include "CommandParser/cmdtlv.h"
#include "cmdcodes.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern graph *topo;

// Generate topology command
static int
show_nw_topology_handler(param_t *param, ser_buff *tlv_buff, op_mode mode) {
    int CMDCODE = -1;
    CMDCODE = EXTRACT_CMD_CODE(tlv_buff);

    switch (CMDCODE) {
        case CMDCODE_SHOW_NW_TOPO:
            dump_nw_graph(topo);
            break;
        default:
            return -1;
    }
    return -1;

}

static int arp_handler(param_t *param, ser_buff *tlv_buff, op_mode mode) {
    int code = EXTRACT_CMD_CODE(tlv_buff);

    tlv_struct *tlv = NULL;
    char *node_name = NULL;
    char *ip_addr = NULL;

    TLV_LOOP_BEGIN(tlv_buff, tlv) {
        if (strncmp(tlv->leaf_id, "node_name", strlen("node_name")) == 0)
            node_name = tlv->value;
        else if (strncmp(tlv->leaf_id, "ip_addr", strlen("ip_addr")) == 0)
            ip_addr = tlv->value;
    } TLV_LOOP_END

    switch(code) {
        case CMDCODE_ARP_ADDR:
            printf("node_name : %s\n", node_name);
            printf("ip address : %s", ip_addr);
        default:
            ;
    }
    return 0;
}


static int validate_node_name(char *leaf_value) {
    node *dev;
    glthread *ptr;
    glthread *lst = &topo->lst;

    ITERATE_GLTHREAD_BEGIN(lst, ptr) {
        dev = glthread_to_node(ptr);
        if (!strcmp(dev->node_name, leaf_value))
            return 1;
    } ITERATE_GLTHREAD_END(&topo->lst, ptr);

    return 0;
}

static int validate_ip_addr(char *leaf_value) {
    char *token = malloc(sizeof(leaf_value));
    strcpy(token, leaf_value);
    int count = 0;
    token = strtok(token, ".");

    while (token != NULL) {
        count++;
        token = strtok(NULL, ".");
    }

    if (count == 4) return 1;
    return 0;
}

void nw_init_cli() {
    init_libcli();

    param_t *root = libcli_get_root_hook();
    param_t *show = libcli_get_show_hook();
    param_t *run = libcli_get_run_hook();

    {
        // Show topology
        static param_t topology;
        init_param(&topology,
                CMD,
                "topology",
                show_nw_topology_handler,
                0,
                INVALID,
                0,
                "Dump Complete Network Topology");
        libcli_register_param(show, &topology);
        set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPO);
    }

    {
        // Assignment of CLI integration
        static param_t node;
        init_param(&node,
                CMD,
                "node",
                0,
                0,
                INVALID,
                0,
                "Nodes of the topology");
        libcli_register_param(run, &node);
        {
            static param_t node_name;
            init_param(&node_name,
                    LEAF,
                    0,
                    0,
                    validate_node_name,
                    STRING,
                    "node_name",
                    "Names of nodes");
            libcli_register_param(&node, &node_name);
            {
                static param_t resolve_arp;
                init_param(&resolve_arp,
                        CMD,
                        "resolve-arp",
                        0,
                        0,
                        INVALID,
                        0,
                        "resolve_arp command");
                libcli_register_param(&node_name, &resolve_arp);
                {
                    static param_t ip_addr;
                    init_param(&ip_addr,
                            LEAF,
                            0,
                            arp_handler,
                            validate_ip_addr,
                            IPV4,
                            "ip_addr",
                            "IPV4 address");
                    libcli_register_param(&resolve_arp, &ip_addr);
                    set_param_cmd_code(&ip_addr, CMDCODE_ARP_ADDR);
                }
            }
        }
    }
}
