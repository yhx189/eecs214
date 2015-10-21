#include "bit_io.h"
#include <stdio.h>

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

node * build_tree(int* weights){
    node* root;

    return root;
}

int find_path(node* root, int label){


}

void encode(FILE *in, FILE *out)
{
    int c;
    bit_out bf = b_attach_out(out);
    int weights[256] = {0};
    
    while ((c = getc(in)) != EOF) {
        //printf("%d\n",c);
        weights[c] += 1;
    }
    int i;
    for(i = 0; i < 256; i++){
       printf("%d : %d\n", i,weights[i]);
    }
    node* huff = build_tree(weights);
    fflush(in);
    while ((c = getc(in)) != EOF) {
        b_write_bits(find_path(huff, c), 7, bf);
    }
    b_detach_out(bf);
}

int usage(const char** argv)
{
    fprintf(stderr, "Usage: %s INFILE OUTFILE\n", argv[0]);

    return 2;
}

