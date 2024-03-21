#ifndef GENERATION_C
#define GENERATION_C

#include "generation.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

void start() {
    printf("\n\n\n\n\n\nMINESWEEPER\n\n\n\n");
    sleep(2);
}

int diff() {
    printf("\n\n\nSelect the difficulty:\n1. Easy (10x8 / 10)\n2. Medium (18x14 / 40)\n3. Hard (24x20 / 99)\n4. Bot!\n");
    printf("Type the number (1/2/3/4):\n");
    int a;
    scanf("%d", &a);
    printf("\n\n\n\n\n\n\n\n\n\n");
    while (a < 1 || a > 4) {
        printf("\n\n\n\n\n\nWrong number!");
        scanf("%d", &a);
    }
    return a;
}

int width(const int *difficulty) {
    switch (*difficulty) {
        case 1:
            return 8;
        case 2:
            return 14;
        case 3:
            return 20;
    }
}

int length(const int *difficulty) {
    switch (*difficulty) {
        case 1:
            return 10;
        case 2:
            return 16;
        case 3:
            return 24;
    }
}

int boom(const int *difficulty) {
    switch (*difficulty) {
        case 1:
            return 10;
        case 2:
            return 40;
        case 3:
            return 99;
    }
}

char **init_field(const int *n, const int *m) {
    char **field = (char **) calloc(*n, sizeof(char *));
    for (int i = 0; i < *n; i++) {
        field[i] = (char *) calloc((*m), sizeof(char));
        for (int j = 0; j < *m; j++)
            field[i][j] = '#';
    }
    return field;
}

int **init_bombs(const int *bombs, const int *n, const int *m, MOVE *first) {
    srand(time(0));
    rand();
    int **bomb_field = (int **) calloc(*n, sizeof(int *));
    for (int i = 0; i < *n; i++) {
        bomb_field[i] = (int *) calloc(*m, sizeof(int));
        for (int j = 0; j < *m; j++)
            bomb_field[i][j] = 0;
    }

    int k = *bombs;
    while (k > 0) {
        int x = rand() % *n;
        int y = rand() % *m;
        while (abs(x - (*first).x) <= 1)
            x = rand() % *n;
        while (abs(y - (*first).y) <= 1)
            y = rand() % *m;
        if (bomb_field[x][y] == 0) {
            bomb_field[x][y] = 100;
            k--;
        }
    }

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *m; j++) {
            if (bomb_field[i][j] == 0) {
                if (i > 0 && j > 0 && bomb_field[i - 1][j - 1] == 100)
                    bomb_field[i][j]++;
                if (i > 0 && bomb_field[i - 1][j] == 100)
                    bomb_field[i][j]++;
                if (i > 0 && j != *m - 1 && bomb_field[i - 1][j + 1] == 100)
                    bomb_field[i][j]++;
                if (j > 0 && bomb_field[i][j - 1] == 100)
                    bomb_field[i][j]++;
                if (j != *m - 1 && bomb_field[i][j + 1] == 100)
                    bomb_field[i][j]++;
                if (i != *n - 1 && j > 0 && bomb_field[i + 1][j - 1] == 100)
                    bomb_field[i][j]++;
                if (i != *n - 1 && bomb_field[i + 1][j] == 100)
                    bomb_field[i][j]++;
                if (i != *n - 1 && j != *m - 1 && bomb_field[i + 1][j + 1] == 100)
                    bomb_field[i][j]++;
            }
        }
    }


    return bomb_field;
}

#endif
