#include "Btree.h"


int get_mode() {
    int mode = 0;
    printf( "Please, choose a mode:\n"
            "1 - game      2 - definition\n"
            "3 - compare   4 - base\n");
    
    while (((scanf("%d", &mode)) != 1) || (mode < 1) || (mode > 4)) 
        BAD_INPUT;
    cleanBuffer();

    return mode;
}

char get_answer() {
    char answer = '\0';
    while ((scanf("%c", &answer)) != 1) 
        BAD_INPUT;
    cleanBuffer();

    return answer;
}

int get_str(char *str) {
    ASSERT(str == NULL, "Void ptr");
    
    char ch = '\0';
    int num = 0;
    while ((ch = getchar()) != '\n') 
        str[num++] = ch;

    str[num] = '\0';

    return 0;
}

//-----------------------------------------------------------------------------


int nodeCtor(BNode_t *node, char *str, BNode_t *left, BNode_t *right) {
    ASSERT(str == NULL, "Void ptr on str");

    strncpy (node->data, str, strlen(str) + 1);
    node->left = left;
    node->right = right;

    return 0;
}


//-----------------------------------------------------------------------------


int r_tree(BNode_t *arr) {
    ASSERT(arr == NULL, "Void ptr");

    FILE *input = fopen ("r-TREE.txt", "rb");
    ASSERT(input == NULL, "Can't open file");

    int nSmbl = countSymbol(input);
    char *str = read_f(input);
    int nlines = countStrings(nSmbl, str);              
    line *astr = (line*)calloc(nlines, sizeof(line));    
    lineCtor(astr, nlines);                             
    scanFile(nSmbl, astr, str); 

    int nNodes = r_node(arr, astr);

    free(str);
    free(astr);
    fclose(input);

    return nNodes;
}


int r_node(BNode_t *arr, line *arr_line) {
    ASSERT(arr == NULL, "Void ptr");
    ASSERT(arr_line == NULL, "Void ptr");

    static int num = 0;
    static int line_num = 0;

    if (strncmp(arr_line[line_num].refonBegin, "{", 1) == 0) {
        line_num++;
        r_node(arr, arr_line);
    }
    
    else if (strncmp(arr_line[line_num].refonBegin, "}", 1) == 0) {
        if (strncmp(arr_line[line_num - 2].refonBegin, "{", 1) == 0) {
            arr[num - 1].left = NULL; 
            arr[num - 1].right = NULL;
            num--;
        }
        line_num++;
    }

    else {
        int num_curr = num;
        strncpy (arr[num].data, arr_line[line_num].refonBegin, arr_line[line_num].lenStr);

        arr[num_curr].left = &arr[++num];
        line_num++;
        r_node(arr, arr_line);

        if (arr[num_curr].left != NULL) {
            arr[num_curr].right = &arr[++num];
            line_num++;
            r_node(arr, arr_line);
        }
    }

    return num;
}


//-----------------------------------------------------------------------------


int w_tree(BNode_t *root) {
    ASSERT(root == NULL, "Void ptr");

    FILE *output = fopen ("r-TREE.txt", "wb");
    ASSERT(output == NULL, "Can't open file");

    w_node(root, output);

    fclose(output);

    return 0;
}

int w_node(BNode_t *node, FILE *output) {
    ASSERT(node == NULL, "Void ptr");
    ASSERT(output == NULL, "Void ptr");

    if ((node->left == NULL) && (node->right == NULL)) {
        if (node->data[strlen(node->data) - 1] == '@') {
            node->data[strlen(node->data) - 1] = '\0';
            fprintf(output, "{\n%s\n}\n", node->data);
            free(node);
        }

        else
            fprintf(output, "{\n%s\n}\n", node->data);
    }

    else {
        fprintf(output, "{\n%s\n", node->data);
        w_node(node->left, output);
        w_node(node->right, output);
        fprintf(output, "}\n");
    }

    return 0;
}


//-----------------------------------------------------------------------------


int treeWalk(BNode_t *node) {
    ASSERT(node == NULL, "Void ptr");

    if ((node->left == NULL) && (node->right == NULL)) {
        give_answer(node);
    }

    else {
        printf("%s?\n", node->data);

        char answer = get_answer();
        
        if (answer == 'y') 
            treeWalk(node->left);
        else
            treeWalk(node->right);
    }

    return 0;
}

int give_answer(BNode_t *node) {
    ASSERT(node == NULL, "Void ptr");

    printf("The answer is %s!\n", node->data);

    char answer = get_answer();
        
    if (answer == 'y') 
        printf("Good job\n");

    else {
        process_wrong_answer(node);
    }

    return 0;
}

int process_wrong_answer(BNode_t *node) {
    ASSERT(node == NULL, "Void ptr");

    printf("Who/What did you think?\n");

    char right[MAX_LENGTH] = {};
    get_str(right);

    printf("What is the difference between %s and %s\n", right, node->data);

    char difference[MAX_LENGTH] = {};
    get_str(difference);

    BNode_t *yes = (BNode_t*)calloc(1, sizeof(BNode_t));
    BNode_t *no = (BNode_t*)calloc(1, sizeof(BNode_t));
    nodeCtor(yes, right, NULL, NULL);
    nodeCtor(no, node->data, NULL, NULL);

    strcat(yes->data, "@\0"); // to understand which nodes should free()
    strcat(no->data, "@\0");

    strncpy(node->data, difference, strlen(difference) + 1);
    node->left = yes;
    node->right = no;

    return 0;
}


//-----------------------------------------------------------------------------

//, style = \"filled\", fillcolor = \"lightgreen\", 
int make_tree_graph(BNode_t *arr, int nNodes) {
    ASSERT(arr == NULL, "Void ptr");
    
    FILE *output = fopen("GRAPH.dot", "wb");
    ASSERT(output == NULL, "Void ptr");

    fprintf(output, "digraph tree {\n");

    for (int num = 0; num < nNodes; num++) {
        if ((arr[num].left == NULL) && (arr[num].right == NULL)) 
            fprintf(output, "\tnode%d [shape = \"record\", label = \"{<d>%s|{NULL|NULL}}\"];\n", num, arr[num].data);

        else 
            fprintf(output, "\tnode%d [shape = \"record\", style = \"filled\", fillcolor = \"lightgoldenrod2\", label = \"{<d>%s|{<l>LEFT|<r>RIGHT}}\"];\n", 
                            num, arr[num].data);
    }

    connect_nodes(&arr[0], output);
    
    fprintf(output, "}");

    fclose(output);

    return 0;
}   

int connect_nodes(BNode_t *node, FILE *output) {
    ASSERT(node == NULL, "Void ptr");

    static int num = 0;
    
    if ((node->left == NULL) && (node->right == NULL)) {
        return 0;
    }

    int num_curr = num;

    fprintf(output, "\tnode%d:<l> -> node%d:<d>;\n", num_curr, num + 1);
    num++;

    connect_nodes(node->left, output);

    fprintf(output, "\tnode%d:<r> -> node%d:<d>;\n", num_curr, num + 1);
    num++;

    connect_nodes(node->right, output);

    return 0;
}