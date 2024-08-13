#include "CommandParser/libcli.h"
#include "graph.h"

extern graph *build_first_topo();
graph *topo = NULL;

int main(int argc, char **argv){
    extern void *nw_init_cli();
    nw_init_cli();
    topo = build_first_topo();
    start_shell();
    return 0;
}
