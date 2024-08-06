#include "net.h"

graph* create_new_graph(char *topology_name);
node* create_graph_node(graph *topology, char *node_name);
void insert_link_between_nodes(node *node1, node *node2,
        char *from_if_name, char *to_if_name, unsigned int cost);

graph *build_first_topo(){

#if 0

                          +----------+
                      0/4 |          |0/0
         +----------------+   R0_re  +---------------------------+
         |     40.1.1.1/24| 122.1.1.0|20.1.1.1/24                |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |40.1.1.2/24                                            |20.1.1.2/24
         |0/5                                                    |0/1
     +---+---+                                              +----+-----+
     |       |0/3                                        0/2|          |
     | R2_re +----------------------------------------------+    R1_re |
     |       |30.1.1.2/24                        30.1.1.1/24|          |
     +-------+                                              +----------+

#endif


graph *topo = create_new_graph("Hello World Generic Graph");
    node *R0_re = create_graph_node(topo, "R0_re");
    node *R1_re = create_graph_node(topo, "R1_re");
    node *R2_re = create_graph_node(topo, "R2_re");

    insert_link_between_nodes(R0_re, R1_re, "eth0/0", "eth0/1", 1);
    insert_link_between_nodes(R1_re, R2_re, "eth0/2", "eth0/3", 1);
    insert_link_between_nodes(R0_re, R2_re, "eth0/4", "eth0/5", 1);

    node_set_lb_addr(R0_re, "122.1.1.0");

    return topo;
}
