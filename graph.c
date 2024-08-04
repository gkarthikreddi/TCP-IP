#include "graph.h"
#include "glthreads/glthread.h"
#include <stdlib.h>
#include <string.h>

graph* create_new_graph(char *topology_name) {
    graph *topology = calloc(1, sizeof(graph));
    strncpy(topology->topology_name, topology_name, 32);
    topology->topology_name[31] = '\0';

    init_glthread(&topology->lst);
    return topology;
}

node* create_graph_node(graph *topology, char *node_name) {
    node *new_node = calloc(1, sizeof(node));
    strncpy(new_node->node_name, node_name, NODE_NAME_SIZE);
    new_node->node_name[NODE_NAME_SIZE-1] = '\0';
    
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
}
