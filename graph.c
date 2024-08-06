#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

graph* create_new_graph(char *topology_name) {
    graph *topo = calloc(1, sizeof(graph));
    strncpy(topo->topology_name, topology_name, 32);
    topo->topology_name[31] = '\0';

    init_glthread(&topo->lst);
    return topo;
}

node* create_graph_node(graph *topology, char *node_name) {
    node *new_node = calloc(1, sizeof(node));
    strncpy(new_node->node_name, node_name, NODE_NAME_SIZE);
    new_node->node_name[NODE_NAME_SIZE-1] = '\0';

    init_node_nw_prop(&new_node->prop);

    init_glthread(&new_node->graph_glue);
    glthread_add_next(&topology->lst, &new_node->graph_glue);
    return new_node;
}


void insert_link_between_nodes(node *node1, node *node2,
        char *from_if_name, char *to_if_name, unsigned int cost) {
    link *wire = calloc(1, sizeof(link));

    strncpy(wire->intf1.if_name, from_if_name, IF_NAME_SIZE);
    strncpy(wire->intf2.if_name, to_if_name, IF_NAME_SIZE);
    wire->intf1.if_name[IF_NAME_SIZE-1] = '\0';
    wire->intf2.if_name[IF_NAME_SIZE-1] = '\0';

    wire->intf1.wire = wire; // Setting back pointer
    wire->intf2.wire = wire; // Setting back pointer

    wire->intf1.att_node = node1;
    wire->intf2.att_node = node2;
    wire->cost = cost;
    
    node1->intf[get_node_intf_available_slot(node1)] = &wire->intf1;
    node2->intf[get_node_intf_available_slot(node2)] = &wire->intf2;

    init_intf_nw_prop(&wire->intf1.prop);
    init_intf_nw_prop(&wire->intf2.prop);

    // Assigning mac addresses
    intf_assign_mac_addr(&wire->intf1);
    intf_assign_mac_addr(&wire->intf2);
}

void dump_graph(graph *topo) {
    glthread *ptr;
    glthread *lst = &topo->lst;
    node *dev;

    printf("Topology name = %s\n", topo->topology_name);

    ITERATE_GLTHREAD_BEGIN(lst, ptr) {
        dev = glthread_to_node(ptr);
        dump_node(dev);
    } ITERATE_GLTHREAD_END(lst, ptr);
}

void dump_node(node *dev) {
    interface *intf;

    printf("Node name = %s: \n", dev->node_name);
    for (int i = 0; i < MAX_INTF_PER_NODE; i++) {
        intf = dev->intf[i];
        if (!intf) break;
        dump_interface(intf);
    }
}

void dump_interface(interface *intf) {
    link *wire = intf->wire;
    node *nbr_node = get_nbr_node(intf);

    printf("Interface Name = %s\n\tNbr Node : %s, Local Node : %s, cost = %u\n",
            intf->if_name, nbr_node->node_name, intf->att_node->node_name, wire->cost);
}
