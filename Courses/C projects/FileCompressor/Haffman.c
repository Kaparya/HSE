#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Haffman.h"

void addToList(NODE* new, NODE** list) {
    if (*list == NULL) {
        (*list) = new;
        (*list)->next = NULL;
        return;
    }

    if ((*list)->freq < new->freq)
        addToList(new, &(*list)->next);
    else {
        new->next = *list;
        (*list) = new;
    }
}

void createTree(NODE** list){
    while ((*list)->next != NULL){
        NODE* new = (NODE*) malloc(sizeof(NODE));
        new->next = NULL;
        new->isSymb = 0;
        new->freq = (*list)->freq + (*list)->next->freq;
        new->left = (*list);
        new->right = (*list)->next;
        (*list) = (*list)->next->next;
        new->left->next = NULL;
        new->right->next = NULL;
        addToList(new, &(*list));
    }
}
void printTree(NODE *root, int level) {
    if (root == NULL)
        return;
    printTree(root->right, level + 1);
    for (int i = 0; i < level * 4; i++)
        printf(" ");
    printf("%d \"%c\"\n", root->freq, root->symb);
    printTree(root->left, level + 1);
}


void goTree(NODE* list, char* translate[256], char now[1000], int nowSize) {
    if (list->isSymb) {
        translate[list->symb] = (char*) calloc(1000, sizeof(char));
        strcpy(translate[list->symb], now);
        return;
    }

    now[nowSize++] = '0';
    now[nowSize] = '\0';
    goTree(list->left, translate, now, nowSize);
    now[nowSize - 1] = '1';
    goTree(list->right, translate, now, nowSize);
    nowSize--;
    now[nowSize] = '\0';
}

void goSaveTree(NODE* tree, FILE** temp) {
    if (tree == NULL)
        return;
    fprintf((*temp), "%d",tree->isSymb);
    if (tree->isSymb) {
        fprintf((*temp), "%d-", tree->symb);
        return;
    }
    goSaveTree(tree->left, &(*temp));
    goSaveTree(tree->right, &(*temp));
}

void saveTree(NODE* tree, int left, FILE** temp) {
    fprintf(*temp, "%d", left);
    goSaveTree(tree, &(*temp));
    fprintf(*temp, "\n%d\n", -5);
}

int makeTree(NODE **tree, FILE** fin) {
    int a, b = 0;
    a = fgetc(*fin);
    if (a == '\n' || a == EOF) {
        printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
        return 1;
    }
    a -= '0';
    if (a){
        if (*tree == NULL || feof(*fin)){
            printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
            return 1;
        }

        int help = fgetc(*fin);
        while (help != '-') {
            if (help < '0' || help > '9' ) {
                printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
                return 1;
            }
            b = b * 10 + help - '0';
            help = fgetc(*fin);
        }
        (*tree)->symb = b;
        (*tree)->next = (*tree)->left = (*tree)->right = NULL;
        (*tree)->isSymb = 1;
        return 0;
    }
    else {
        (*tree)->next = NULL;
        (*tree)->right = (NODE*) malloc(sizeof(NODE));
        (*tree)->left = (NODE*) malloc(sizeof(NODE));
        (*tree)->isSymb = 0;
    }
    return makeTree(&((*tree)->left), &(*fin)) || makeTree(&((*tree)->right), &(*fin));
}

int recoverTree(NODE** tree, int* left, FILE** fin) {
    if (feof(*fin)) {
        printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
        return 1;
    }

    *left = fgetc(*fin) - '0';
    if (*left < 0 || *left > 7) {
        printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
        return 1;
    }

    *tree = (NODE*) malloc(sizeof(NODE));
    (*tree)->isSymb = 0;
    if (makeTree(tree, &(*fin))) {
        return 1;
    }
    int help;

    if (feof(*fin)) {
        printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
        return 1;
    }

    fscanf((*fin), "%d", &help);
    if (help != -5){
        printf("\n\n\n\nCOMPRESSED DATA ERROR\n\n\n\n");
        return 1;
    }

    return 0;
}

void freeTree(NODE **tree) {
    if (*tree == NULL)
        return;
    freeTree(&((*tree)->left));
    freeTree(&((*tree)->right));
    free(*tree);
}
