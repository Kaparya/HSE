#include "Constants.h"

void addToList(NODE* new, NODE** list);

void createTree(NODE** list);
void printTree(NODE* root, int level);

void goTree(NODE* list, char* translate[256], char now[1000], int nowSize);

void saveTree(NODE* tree, int left, FILE** temp);
int recoverTree(NODE** tree, int* left, FILE** fin);

void freeTree(NODE** tree);