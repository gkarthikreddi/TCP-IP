#include "glthreads/glthread.h"
#include <assert.h>

#define IF_NAME_SIZE    16
#define NODE_NAME_SIZE  16
#define MAX_INTF_PER_NODE 10

// Forward Declaration
typedef struct link link;
typedef struct node node;

typedef struct interface {
    char if_name[IF_NAME_SIZE];
    struct node *att_node;
    struct link *wire;
} interface;

struct link {
    interface intf1;
    interface intf2;
    unsigned int cost;
};

struct node {
    char node_name[NODE_NAME_SIZE];
    interface *intf[MAX_INTF_PER_NODE];
    glthread graph_glue;
};

typedef struct graph {
    char topology_name[32];
    glthread lst;
} graph;

// Helper functions. (Assignment 1)
static inline node* get_nbr_node(interface *intf) {
    assert(intf->wire);
    assert(intf->att_node);

    if (&intf->wire->intf1 == intf) return intf->wire->intf2.att_node;
    else return intf->wire->intf1.att_node;
}

static inline int get_node_intf_available_slot(node *dev) {
    for (int i = 0 ; i < MAX_INTF_PER_NODE; i++)
        if (!dev->intf[i]) return i;
    
    return -1;

// APIs
graph* create_new_graph(char *topology_name);
node* create_graph_node(graph *topology, char *node_name);
void insert_link_between_nodes(node *node1, node *node2,
        char *from_if_name, char *to_if_name, unsigned int cost);
}

// (Assignment 2)
static inline interface* get_node_if_by_name(node *dev, char *if_name) {
    for (int i = 0; i < MAX_INTF_PER_NODE; i++)
        if (dev->intf[i]->if_name == if_name) return dev->intf[i];

    return NULL;
}

#define offset(struct_name, fieldname)     \
    (char *)&((struct_name *)0)->fieldname

#define glthread_to_node(ptr)                                           \
    ((struct node*)((char *)(ptr) - offset(struct node, graph_glue)))

static inline node* get_node_by_node_name(graph *topo, char *node_name) {
    glthread *lst = &topo->lst;
    glthread *ptr = NULL;
    node *curr;
    ITERATE_GLTHREAD_BEGIN(lst, ptr) {
        curr = glthread_to_node(ptr);
        if (curr->node_name == node_name) return curr;
    } ITERATE_GLTHREAD_END(lst, ptr);
    return nullptr;
}
