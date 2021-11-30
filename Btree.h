#ifndef BTREE_H_
#define BTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "oneggin.h"

const int NUM_OF_NODES = 10;
const int MAX_LENGTH = 100;

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                        return 1;                                                                       \
                                    }                                                                                   \
                                } while (0)
#endif

#ifndef BAD_INPUT
#define BAD_INPUT   do {                                \
                        printf("Incorrect input\n");    \
                        cleanBuffer();                  \
                    } while(0)
#endif

struct BNode_t {
    BNode_t *left;
    BNode_t *right;
    char data[MAX_LENGTH];
};

int get_mode();
char get_answer();
int get_str(char *str);

int nodeCtor(BNode_t *node, char *str, BNode_t *left, BNode_t *right);

int r_tree(BNode_t *arr);
int r_node(BNode_t *arr, line *arr_line);

int w_tree(BNode_t *arr);
int w_node(BNode_t *nodeCtor, FILE *output);

int make_tree_graph(BNode_t *arr, int nNodes);
int connect_nodes(BNode_t *node, FILE *output);

int treeWalk(BNode_t *node);
int give_answer(BNode_t *node);
int process_wrong_answer(BNode_t *node);

int get_str(char *str);

#endif
