#ifndef GENERATION_H
#define GENERATION_H

typedef struct move {
    char com;
    int x, y;
} MOVE;

void start();

int diff();

int width(const int *difficulty);

int length(const int *difficulty);

int boom(const int *difficulty);

char **init_field(const int *n, const int *m);

int **init_bombs(const int *bombs, const int *n, const int *m, MOVE *first);

#endif