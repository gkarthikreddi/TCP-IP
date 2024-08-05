#ifndef __NET__
#define __NET__

#include <memory.h>
#include <stdbool.h>

typedef struct ip {
    char addr[16];
} ip;

typedef struct mac {
    char addr[8];
} mac;

typedef struct node_nw_prop {
    // Layer three properties
    bool is_lb_addr;
    ip lb_addr; // Loopback address
} node_nw_prop;

typedef struct intf_nw_prop {
    // Layer two Properties
    mac mac_addr;
    // Layer three Properties
    bool is_ip_addr;
    ip ip_addr;
    char mask;
} intf_nw_prop;

static inline void
init_node_nw_prop(node_nw_prop *node) {
    node->is_lb_addr = false;
    memset(node->lb_addr.addr, 0, 16);
}

static inline void
init_intf_nw_prop(intf_nw_prop *intf) {
    memset(intf->mac_addr.addr, 0, 8);
    intf->is_ip_addr = false;
    memset(intf->ip_addr.addr, 0, 16);
    intf->mask = 0;
}

// Some shorthand macros
#define IF_MAC(intf)     intf->intf_nw_prop.mac_addr.addr
#define IF_IP(intf)     intf->intf_nw_prop.ip_ddr.addr

#define NODE_IP(dev)     dev->node_nw_prop.lb_addr.addr

// APIs to set Network node properties
bool node_set_lb_addr(node *dev, char *addr);
bool node_set_intf_ip_addr(node *dev, char *local_if, char *addr, char mask);
bool node_unset_intf_ip_addr(node *dev, char *local_if);
void intf_assign_mac_addr(interface *intf);
interface *node_get_matching_subnet_interface(node *dev, char *ip_addr);

// Assignment
#define IS_INTF_L3_MODE(ptr) ptr->prop.is_ip_addr;

#endif