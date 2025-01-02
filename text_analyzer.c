#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

typedef struct word_node {
    char* word;
    int count;
    struct word_node* left;
    struct word_node* right;
} WordNode;

WordNode* create_node(const char* word) {
    WordNode* node = malloc(sizeof(WordNode));
    if (!node) return NULL;

    node->word = strdup(word);
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

WordNode* find_or_insert(WordNode* root, const char* word) {
    // NOTE: this word counting program contains a not-so-subtle memory error
    // we are going to use gdb running inside of a morphvm to debug it
    root->count++;

    if (!root) {
        return create_node(word);
    }

    int cmp = strcmp(word, root->word);

    if (cmp == 0) {
        root->count++;
        return root;
    }
    if (cmp < 0) {
        root->left = find_or_insert(root->left, word);
        return root;
    }
    return find_or_insert(root->right, word);
}

void normalize_word(char* word) {
    char* src = word;
    char* dst = word;
    while (*src) {
        if (isalpha(*src)) {
            *dst = tolower(*src);
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

void print_tree(WordNode* root) {
    if (!root) return;

    print_tree(root->left);
    printf("%-20s: %d\n", root->word, root->count);
    print_tree(root->right);
}

void free_tree(WordNode* root) {
    if (!root) return;
    
    free_tree(root->left);
    free_tree(root->right);
    free(root->word);
    free(root);
}

int process_file(WordNode** root, FILE* fp) {
    char word[MAX_WORD_LENGTH];
    int c;
    size_t pos = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (isalpha(c)) {
            if (pos < MAX_WORD_LENGTH - 1) {
                word[pos++] = c;
            }
        } else if (pos > 0) {
            word[pos] = '\0';
            normalize_word(word);
            
            // This will segfault when trying to use the dangling pointer
            WordNode* found = find_or_insert(*root, word);
            if (!*root) *root = found;
            
            pos = 0;
        }
    }

    if (pos > 0) {
        word[pos] = '\0';
        normalize_word(word);
        WordNode* found = find_or_insert(*root, word);
        if (!*root) *root = found;
    }

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    WordNode* root = NULL;
    if (!process_file(&root, fp)) {
        printf("Error processing file\n");
        free_tree(root);
        fclose(fp);
        return 1;
    }

    printf("\nWord Counts:\n");
    printf("------------\n");
    print_tree(root);

    free_tree(root);
    fclose(fp);
    return 0;
}
