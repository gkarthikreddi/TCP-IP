#include "graph.h"
#include "utils.h"
#include <memory.h>

void intf_assign_mac_addr(interface *intf){
    memset(IF_MAC(intf), 0, 48);
    strcpy(IF_MAC(intf), intf->att_node->node_name);
    strcat(IF_MAC(intf), intf->if_name);
}

bool node_set_lb_addr(node *dev, char *addr) {
    assert(addr);
    dev->prop.is_lb_addr = true;
    strncpy(NODE_IP(dev), addr, 16);
    NODE_IP(dev)[15] = '\0';

    return true;
}

bool node_set_intf_ip_addr(node *dev, char *local_if, char *addr, char mask) {
    interface *intf = get_node_if_by_name(dev, local_if);
    if (!intf) assert(0);

    intf->prop.is_ip_addr = true;
    strncpy(IF_IP(intf), addr, 16);
    IF_IP(intf)[15] = '\0';
    intf->prop.mask = mask;

    return true;
}

bool node_unset_intf_ip_addr(node *dev, char *local_if) {
    interface *intf = get_node_if_by_name(dev, local_if);
    if (!intf) assert(0);

    init_intf_nw_prop(&intf->prop);

    return true;
}

interface *node_get_matching_subnet_interface(node *dev, char *ip_addr) {
    unsigned int i = 0;
    interface *intf;

    char *intf_addr = NULL;
    char mask;
    char intf_subnet[16];
    char subnet2[16];

    for( ; i < MAX_INTF_PER_NODE; i++){
        intf = dev->intf[i];
        if(!intf) return NULL;

        if(intf->prop.is_ip_addr == false)
            continue;
        
        intf_addr = IF_IP(intf);
        mask = intf->prop.mask;

        memset(intf_subnet, 0 , 16);
        memset(subnet2, 0 , 16);
        apply_mask(intf_addr, mask, intf_subnet);
        apply_mask(ip_addr, mask, subnet2);
        
        if(strncmp(intf_subnet, subnet2, 16) == 0){
            return intf;
        }
    }
}
