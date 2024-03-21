#ifndef SOLUTION_C
#define SOLUTION_C

#include "solution.h"
#include <unistd.h>
#include <stdio.h>

void print_solution(char **field, const int *n, const int *m) {
    printf("\n\n\n\n\n\n!SOLUTION!\n");
    printf("%3.c", '+');
    for (int j = 0; j < *m; j++) {
        printf("%3.d", j + 1);
    }
    printf("%3.c\n", '+');
    for (int i = 0; i < *n; i++) {
        printf("%3.d", i + 1);
        for (int j = 0; j < *m; j++) {
            printf("%3.c", field[i][j]);
        }
        printf("%3.d", i + 1);
        printf("\n");
    }
    printf("%3.c", '+');
    for (int j = 0; j < *m; j++) {
        printf("%3.d", j + 1);
    }
    printf("%3.c\n!SOLUTION!\n", '+');
}

void solving(char **field, const int **field_bomb, const int *n, const int *m) {
    printf("SOLUTION:\n");

    usleep(1000);
    for (int i = 0; i < *n; i++) {
        int fl = 0;
        for (int j = 0; j < *m; j++) {
            if (field[i][j] == '#') {
                fl = 1;
                if (field_bomb[i][j] != 100 && field_bomb[i][j] != 0)
                    field[i][j] = (char) ('0' + field_bomb[i][j]);
                else if (field_bomb[i][j] != 100)
                    field[i][j] = '.';
                print_solution(field, n, m);
            }
        }
        if (fl)
            sleep(2);
    }
    sleep(2);
    for (int i = 0; i < *n; i++) {
        int fl = 0;
        for (int j = 0; j < *m; j++) {
            if (field[i][j] == '#' && field_bomb[i][j] == 100) {
                field[i][j] = 'F';
                fl = 1;
                print_solution(field, n, m);
            }
        }
        if (fl)
            sleep(2);
    }
}

#endif