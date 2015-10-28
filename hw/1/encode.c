#include "bit_io.h"
#include <stdio.h>
#include <stdlib.h>
#define INF 1000000
void encode(FILE* in, FILE* out);
int usage(const char** argv);

int main(int argc, const char** argv)
{
    if (argc != 3) return usage(argv);

    FILE* in = fopen(argv[1], "r");
    if (!in) goto in_fail;

    FILE* out = fopen(argv[2], "w");
    if (!out) goto out_fail;

    encode(in, out);

    fclose(out);
    fclose(in);
    return 0;

out_fail:
    fclose(in);

in_fail:
    perror(argv[0]);
    return 1;
}

typedef struct nodev{
    struct nodev * left;
    struct nodev * right;
    int weight;
    int value;
} node;

node * find_small(node *forest[], int tree_cnt, int min){
    int i = 0, smaller;
    while(forest[i]->weight == -1)
        i++;
    smaller = i;
    if(i == min){
        i++;
        while(forest[i]->weight == -1)
            i++;
        smaller = i;
    }
    printf("i = %d\n", i);
    for(i = 1; i < tree_cnt; i++){
        if(forest[i]->weight == -1)
            continue;
        if(i == min)
            continue;
        if(forest[i]->weight < forest[smaller]->weight)
            smaller = i;
    }
    printf("the smallest index is %d\n", smaller);
    return smaller;
}

node * build_tree(int* weights){
    node* root, *tmp;
    node* forest[256];
    int i, tree_cnt = 0;
    int tree_1, tree_2;
    
    int  new_weight[256];
    // disgard those trees with weight 0
    for(i=0; i < 256; i++){
       if(weights[i] > 0 ){
           //new_weight[tree_cnt] = malloc(sizeof(int));
           new_weight[tree_cnt] = weights[i];
           tree_cnt ++;
       }
    }
    printf("tree cnt is: %d\n",tree_cnt);
    for(i = 0; i < tree_cnt; i++){
        forest[i] = malloc(sizeof(node));
        forest[i]->value = i;
        forest[i]->weight = new_weight[i];
        forest[i]->left = NULL;
        forest[i]->right = NULL;
    }
    printf("build tree initialized\n");
    while(tree_cnt > 1){
        tree_1 = find_small(forest,tree_cnt, -1);
        tree_2 = find_small(forest, tree_cnt, tree_1);
        printf("tree_1 is %d, tree_2 is %d\n", tree_1, tree_2);
        tmp = forest[tree_1];
        forest[tree_1] = malloc(sizeof(node));
        forest[tree_1]->value = INF;
        forest[tree_1]->weight = tmp->weight + forest[tree_2]->weight;
        forest[tree_1]->left = forest[tree_2];
        forest[tree_1]->right = tmp;
        forest[tree_2]->weight = -1;
        tree_cnt --;
    
    }
    printf("build tree finished\n");
    return forest[tree_1];
}

void build_table(char** code_table, node* root, char* root_code){
   if(root->value < INF){
        code_table[root->value] = root_code;
        printf("current root code is %s\n", root_code);
   }else{
        build_table(code_table,root->left, strcat("0",root_code));
        build_table(code_table, root->right, strcat("1", root_code));
   }
    return;
}

char* find_path(char **code_table, int label){
    return code_table[label];

}

void encode(FILE *in, FILE *out)
{
    int c;
    bit_out bf = b_attach_out(out);
    int weights[256] = {0};
    int cnt = 0;
    char* code_table[256] = {""}; 
    while ((c = getc(in)) != EOF) {
        //printf("%d\n",c);
        weights[c] += 1;
    }
    int i;
    for(i = 0; i < 256; i++){
       if(weights[i]!=0)
           cnt++;
       printf("%d : %d\n", i,weights[i]);
    }
    printf("the total number of ascii is :%d\n", cnt);
    
    
    node* huff = build_tree(weights);
    
    build_table(code_table, huff, "0");
    for(i = 0; i < 256; i++){
        printf("code table[%d]: %s\n",i, code_table[i]);
    
    }
    fflush(in);
    while ((c = getc(in)) != EOF) {
        b_write_bits(find_path(code_table, c), 7, bf);
    }
    b_detach_out(bf);
}

int usage(const char** argv)
{
    fprintf(stderr, "Usage: %s INFILE OUTFILE\n", argv[0]);

    return 2;
}

