#include "graph.h"

extern graph *build_first_topo();

int main(int argc, char **argv){
    graph *topo = build_first_topo();
    dump_nw_graph(topo);
    return 0;
}
