#ifndef GAME_C
#define GAME_C

#include "game.h"
#include "solution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void print_field(const char **field, const int *bombs, const int *move, const int *n, const int *m) {
    printf("\n\n\n\n");
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
        if (i == *n / 2 - 1)
            printf("      Bombs left: %d (according to your flags)", *bombs);
        if (i == *n / 2)
            printf("      Number of moves: %d", *move);
        printf("\n");
    }
    printf("%3.c", '+');
    for (int j = 0; j < *m; j++) {
        printf("%3.d", j + 1);
    }
    printf("%3.c\n", '+');
}

MOVE movement(const int *n, const int *m) {
    printf("\nSelect the cell:\n? x y - Put flag in [x][y]\n! x y - Open cell [x][y]\n$ x y - Remove flag from [x][y]\nq - Quit\n");
    MOVE result;
    while (result.com != '!' && result.com != '?' && result.com != '$' && result.com != 'q')
        scanf("%c", &result.com);
    if (result.com == 'q') {
        return result;
    }
    scanf("%d%d", &result.x, &result.y);
    if (result.com != '!' && result.com != '?' && result.com != '$') {
        printf("Wrong first symbol\n");
        return movement(n, m);
    }
    if (result.x <= 0 || result.x > *n) {
        printf("Wrong first number\n");
        return movement(n, m);
    }
    if (result.y <= 0 || result.y > *m) {
        printf("Wrong second number\n");
        return movement(n, m);
    }
    return result;
}

void BOOM(char **field, const int **field_bomb, const int *n, const int *m, int BOT) {
    srand(time(0));
    rand();
    printf("\n\n\n\n\n\nBOOOM!!!\n");
    sleep(2);
    printf("\nDo you want to look at the solution? (1/0)");
    int a;
    if (!BOT)
        scanf("%d", &a);
    else {
        a = rand() % 2;
        printf("\n%d\n\n", a);
    }
    if (a)
        solving(field, field_bomb, &(*n), &(*m));
    printf("\n\n\n\n\n\nGAME OVER\n");
}

void WIN(int *moves) {
    printf("\n\n\n\n\n\n\nYou WON in %d moves!\n", *moves);
}

int check_flag(const int **field_bomb, MOVE *now) {
    if (field_bomb[now->x][now->y] == 100)
        return 1;
    return 0;
}

int check(const int **field_bomb, MOVE *now, char **field, int *closed, int *n, int *m) {
    if (field_bomb[now->x][now->y] == 100)
        return 1;
    if (field_bomb[now->x][now->y] != 0) {
        field[now->x][now->y] = (char) ('0' + field_bomb[now->x][now->y]);
        (*closed)--;
        return 0;
    }

    MOVE array[500];
    int cori = 0, addi = 1;
    array[0] = *now;

    while (cori != addi) {
        MOVE help = array[cori];
        cori++;
        if (field[help.x][help.y] == '#') {
            (*closed)--;
            field[help.x][help.y] = '.';
        }
        if (help.x > 0 && help.y > 0 && field[help.x - 1][help.y - 1] == '#' &&
            field_bomb[help.x - 1][help.y - 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x - 1][help.y - 1] == 0) {
                MOVE h = {'1', help.x - 1, help.y - 1};
                array[addi++] = h;
                field[help.x - 1][help.y - 1] = '.';
            } else
                field[help.x - 1][help.y - 1] = (char) ('0' + field_bomb[help.x - 1][help.y - 1]);
        }
        if (help.x > 0 && field[help.x - 1][help.y] == '#' && field_bomb[help.x - 1][help.y] != 100) {
            (*closed)--;
            if (field_bomb[help.x - 1][help.y] == 0) {
                MOVE h = {'1', help.x - 1, help.y};
                array[addi++] = h;
                field[help.x - 1][help.y] = '.';
            } else
                field[help.x - 1][help.y] = (char) ('0' + field_bomb[help.x - 1][help.y]);
        }
        if (help.x > 0 && help.y < *n - 1 && field[help.x - 1][help.y + 1] == '#' &&
            field_bomb[help.x - 1][help.y + 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x - 1][help.y + 1] == 0) {
                MOVE h = {'1', help.x - 1, help.y + 1};
                array[addi++] = h;
                field[help.x - 1][help.y + 1] = '.';
            } else
                field[help.x - 1][help.y + 1] = (char) ('0' + field_bomb[help.x - 1][help.y + 1]);
        }
        if (help.y > 0 && field[help.x][help.y - 1] == '#' && field_bomb[help.x][help.y - 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x][help.y - 1] == 0) {
                MOVE h = {'1', help.x, help.y - 1};
                array[addi++] = h;
                field[help.x][help.y - 1] = '.';
            } else
                field[help.x][help.y - 1] = (char) ('0' + field_bomb[help.x][help.y - 1]);
        }
        if (help.y < *m - 1 && field[help.x][help.y + 1] == '#' && field_bomb[help.x][help.y + 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x][help.y + 1] == 0) {
                MOVE h = {'1', help.x, help.y + 1};
                array[addi++] = h;
                field[help.x][help.y + 1] = '.';
            } else
                field[help.x][help.y + 1] = (char) ('0' + field_bomb[help.x][help.y + 1]);
        }
        if (help.x < *n - 1 && help.y > 0 && field[help.x + 1][help.y - 1] == '#' &&
            field_bomb[help.x + 1][help.y - 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x + 1][help.y - 1] == 0) {
                MOVE h = {'1', help.x + 1, help.y - 1};
                array[addi++] = h;
                field[help.x + 1][help.y - 1] = '.';
            } else
                field[help.x + 1][help.y - 1] = (char) ('0' + field_bomb[help.x + 1][help.y - 1]);
        }
        if (help.x < *n - 1 && field[help.x + 1][help.y] == '#' && field_bomb[help.x + 1][help.y] != 100) {
            (*closed)--;
            if (field_bomb[help.x + 1][help.y] == 0) {
                MOVE h = {'1', help.x + 1, help.y};
                array[addi++] = h;
                field[help.x + 1][help.y] = '.';
            } else
                field[help.x + 1][help.y] = (char) ('0' + field_bomb[help.x + 1][help.y]);
        }
        if (help.x < *n - 1 && help.y < *m - 1 && field[help.x + 1][help.y + 1] == '#' &&
            field_bomb[help.x + 1][help.y + 1] != 100) {
            (*closed)--;
            if (field_bomb[help.x + 1][help.y + 1] == 0) {
                MOVE h = {'1', help.x + 1, help.y + 1};
                array[addi++] = h;
                field[help.x + 1][help.y + 1] = '.';
            } else
                field[help.x + 1][help.y + 1] = (char) ('0' + field_bomb[help.x + 1][help.y + 1]);
        }
    }

    return 0;
}

#endif
