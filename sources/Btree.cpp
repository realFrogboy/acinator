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
    nNodes++;

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
        strncpy(arr[num].data, arr_line[line_num].refonBegin, arr_line[line_num].lenStr);

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


int make_tree_graph(BNode_t *arr, int nNodes) {
    ASSERT(arr == NULL, "Void ptr");
    
    FILE *output = fopen("GRAPH_tree.dot", "wb");
    ASSERT(output == NULL, "Void ptr");

    fprintf(output, "digraph tree {\n");

    for (int num = 0; num < nNodes; num++) {
        if ((arr[num].left == NULL) && (arr[num].right == NULL)) 
            fprintf(output, "\tnode%d [shape = \"record\", style = \"filled\", fillcolor = \"greenyellow\", label = \"{<d>%s|{NULL|NULL}}\"];\n", num, arr[num].data);

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


//-----------------------------------------------------------------------------


int defin(BNode_t *aNodes) {
    ASSERT(aNodes == NULL, "Void ptr");

    printf("Enter the object\n");

    char obj[MAX_LENGTH] = {};
    scanf("%s", obj);

    struct chrct chr = {};
    chrctCtor(&chr);

    cmp_node(&aNodes[0], obj, &chr);

    print_def(aNodes, &chr);

    chrctDtor(&chr);

    return 0;
}

int cmp_node(BNode_t *node, const char *str, chrct *chr) {
    ASSERT(node == NULL, "Void ptr");
    ASSERT(str == NULL, "Void ptr");

    static int num = 0;
    int num_curr = num;

    if (strncmp(node->data, str, strlen(str)) == 0){
        num = 0; 
        return 2;
    }

    if ((node->left == NULL) && (node->right == NULL)) 
        return 0;

    stackPush(&chr->stk_p, num_curr);
    num++;

    int tmp = cmp_node(node->left, str, chr);
    if (tmp == 2) return 2;

    stackPop(&chr->stk_p);

    stackPush(&chr->stk_n, num_curr);
    num++;

    tmp = cmp_node(node->right, str, chr);
    if (tmp == 2) return 2;

    stackPop(&chr->stk_n);

    return 0;
}

int print_def(BNode_t *arr, chrct *chr) {
    ASSERT(arr == NULL, "Void ptr");
    ASSERT(chr == NULL, "Void ptr");

    print_stats(arr, &chr->stk_p);

    printf("\n and NOT - ");

    print_stats(arr, &chr->stk_n);

    printf("\n");

    return 0;
}

int print_stats(BNode_t *arr, Stack *stk) {
    ASSERT(arr == NULL, "Void ptr");
    ASSERT(stk == NULL, "Void ptr");

    int size_stk = stk->Size;
    for (int num = 0; num < size_stk; num++) {
        int idx =(int)stk->data[stk->Size - 1];
        printf("%s, ", arr[idx].data);
        stackPop(stk);
    }

    return 0;
}

int chrctCtor(chrct *strc) {
    ASSERT(strc == NULL, "Void ptr");

    stackCtor(&strc->stk_p);
    stackCtor(&strc->stk_n);

    return 0;
}

int chrctDtor(chrct *strc) {
    ASSERT(strc == NULL, "Void ptr");

    stackDtor(&strc->stk_p);
    stackDtor(&strc->stk_n);

    return 0;
}


//-----------------------------------------------------------------------------


int cmp_obj(BNode_t *aNodes) {
    ASSERT(aNodes == NULL, "Void ptr");

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

    print_sim(&chr_f.stk_p, &chr_s.stk_p, aNodes);
    printf("\n");

    printf("and NOT ");

    print_sim(&chr_f.stk_n, &chr_s.stk_n, aNodes);
    printf("\n");

    printf(" but %s is ", obj_f);
    print_def(aNodes, &chr_f);

    printf("%s is ", obj_s);
    print_def(aNodes, &chr_s);

    chrctDtor(&chr_f);
    chrctDtor(&chr_s);
    return 0;
}


int print_sim(Stack *stk_f, Stack *stk_s, BNode_t *aNodes) {
    ASSERT(aNodes == NULL, "Void ptr");

    List list_f = list_ctor();
    List list_s = list_ctor();

    list_w(stk_f, &list_f);
    list_w(stk_s, &list_s);

    int tail_f = list_f.tail;
    int tail_s = list_s.tail;

    for (int num = 1; num <= tail_f; num++) {

        for (int idx = 1; idx <= tail_s; idx++) {

            if ((list_s.arr[idx].next != -1) && (list_f.arr[num].next != -1) &&
                 isequal(list_f.arr[num].data, list_s.arr[idx].data) == 1) {

                printf("%s ", aNodes[(int)list_f.arr[num].data].data);
                del_elem(&list_f, num);
                del_elem(&list_s, idx);
                
            }

        }

    }

    list_r(stk_f, &list_f);
    list_r(stk_s, &list_s);

    list_dtor(&list_f);
    list_dtor(&list_s);

    return 0;
}

int list_w(Stack *stk, List *list) {
    ASSERT(stk == NULL, "Void ptr");
    ASSERT(list == NULL, "Void ptr");

    int Size = stk->Size;
    for (int num = 0; num < Size; num++) {
        int idx =(int)stk->data[stk->Size - 1];
        ins_at_the_end(list, idx);
        stackPop(stk);
    }

    return 0;
}

int list_r(Stack *stk, List *list) {
    ASSERT(stk == NULL, "Void ptr");
    ASSERT(list == NULL, "Void ptr");

    for (unsigned num = 1; num <= list->tail; num++)
        if (list->arr[num].next != -1)
            stackPush(stk, list->arr[num].data);
    
    return 0;
}