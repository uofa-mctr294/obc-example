#include <stdio.h>

#include "OBC.hpp"

int main(int argc, char *argv[]) {
    int obcport;
    OBC obc;

    if (argc == 1) {
        fprintf(stderr, "%s: missing OBC port\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%d", &obcport) != 1) {
        fprintf(stderr, "%s: invalid port: %s\n", argv[0], argv[1]);
    }

    if (obc.init(obcport, obcport+1) != 0) {
        fprintf(stderr, "in real life we would actually do something here\n");
        return 1;
    }

    obc.handleCmds();

    return 0;
}    
