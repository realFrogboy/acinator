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
    else if (mode == 3) {
        printf("Enter two objects:");

        char obj_f[MAX_LENGTH] = {};
        char obj_s[MAX_LENGTH] = {};

        scanf("%s %s", obj_f, obj_s);

        struct chrct chr_f = {};
        chrctCtor(&chr_f);
        struct chrct chr_s = {};
        chrctCtor(&chr_s);

        cmp_node(&aNodes[0], obj_f, &chr_f);
        cmp_node(&aNodes[0], obj_s, &chr_s);

        printf("%s looks like a %s because ", obj_f, obj_s);

        print_sim(chr_f.stk_p, chr_s.stk_p, aNodes);

        printf("and NOT");

        print_sim(chr_f.stk_n, chr_s.stk_n, aNodes);

        printf(" but %s is ", obj_f);
        print_def(aNodes, &chr_f);

        printf("%s is ", obj_s);
        print_def(aNodes, &chr_s);

        chrctDtor(&chr_f);
        chrctDtor(&chr_s);
        return 0;
    }

    w_tree(&aNodes[0]);

    return 0;
}