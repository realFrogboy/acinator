#include "Btree.h"

int DEBUG_LEVEL = 3; //for stack

int main() {
    BNode_t aNodes[NUM_OF_NODES] = {}; 

    int nNodes = r_tree(aNodes);
    nNodes++;

    make_tree_graph(aNodes, nNodes);
    /*for (int num = 0; num < nNodes; num++) {
        printf("%s\n", aNodes[num].data);
    }*/

    int mode = get_mode();

    if (mode == 1) {
        treeWalk(&aNodes[0]);
    }
    else if (mode == 2) {
        defin(aNodes);
    }

    w_tree(&aNodes[0]);

    return 0;
}