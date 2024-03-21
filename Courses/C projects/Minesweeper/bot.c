#ifndef BOT_C
#define BOT_C

#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "game.h"
#include "unistd.h"
#include "bot.h"

int bot(){
    srand(time(0));
    rand();
    printf("\n\n\nBOT IS PLAYING\n\n\n");
    printf("\n\n\nSelect the difficulty:\n1. Easy (10x8 / 10)\n2. Medium (18x14 / 40)\n3. Hard (24x20 / 99)\n");
    printf("Type the number (1/2/3):\n");
    int a = rand() % 3 + 1;
    printf("%d", a);
    printf("\n\n\n\n\n\n\n\n\n\n");
    return a;
}

int neighbours_b(int i, int j, const char **field, int n, int m) {
    int ans = 0;
    for (int i1 = -1; i1 <= 1; i1++){
        for (int j1 = -1; j1 <= 1; j1++){
            if (i + i1 >= 0 && i + i1 < n && j + j1 >= 0 && j + j1 < m && (field[i + i1][j + j1] == '#' || field[i + i1][j + j1] == 'F'))
                ans++;
        }
    }
    return ans;
}

int neighbours_f(int i, int j, const char **field, int n, int m) {
    int ans = 0;
    for (int i1 = -1; i1 <= 1; i1++){
        for (int j1 = -1; j1 <= 1; j1++){
            if (i + i1 >= 0 && i + i1 < n && j + j1 >= 0 && j + j1 < m && field[i + i1][j + j1] == 'F')
                ans++;
        }
    }
    return ans;
}

MOVE bot_movement_f(int n, int m) {
    srand(time(0));
    rand();
    printf("\nSelect the cell:\n? x y - Put flag in [x][y]\n! x y - Open cell [x][y]\n$ x y - Remove flag from [x][y]\nq - Quit\n");
    MOVE result;
    result.com = '!';
    result.x = rand() % n + 1;
    result.y = rand() % m + 1;
    printf("%c %d %d\n", result.com, result.x, result.y);
    sleep(1);
    return result;
}

MOVE bot_movement(int n, int m, const char** field) {
    printf("\nSelect the cell:\n? x y - Put flag in [x][y]\n! x y - Open cell [x][y]\n$ x y - Remove flag from [x][y]\nq - Quit\n");
    MOVE result;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (field[i][j] != '.' && field[i][j] != '#'){
                if (neighbours_b(i, j, field, n, m) == field[i][j] - '0'){
                    for (int i1 = -1; i1 <= 1; i1++){
                        for (int j1 = -1; j1 <= 1; j1++){
                            if (i + i1 >= 0 && i + i1 < n && j + j1 >= 0 && j + j1 < m && field[i + i1][j + j1] == '#')
                            {
                                result.com = '?';
                                result.x = i + i1 + 1;
                                result.y = j + j1 + 1;
                                printf("%c %d %d\n", result.com, result.x, result.y);
                                sleep(1);
                                return result;
                            }
                        }
                    }
                }
                else if (neighbours_f(i, j, field, n, m) == field[i][j] - '0'){
                    for (int i1 = -1; i1 <= 1; i1++){
                        for (int j1 = -1; j1 <= 1; j1++){
                            if (i + i1 >= 0 && i + i1 < n && j + j1 >= 0 && j + j1 < m && field[i + i1][j + j1] == '#')
                            {
                                result.com = '!';
                                result.x = i + i1 + 1;
                                result.y = j + j1 + 1;
                                printf("%c %d %d\n", result.com, result.x, result.y);
                                sleep(1);
                                return result;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (field[i][j] != '.' && field[i][j] != '#'){
                if (neighbours_f(i, j, field, n, m) == field[i][j] - '0'){
                    for (int i1 = -1; i1 <= 1; i1++){
                        for (int j1 = -1; j1 <= 1; j1++){
                            if (i + i1 >= 0 && i + i1 < n && j + j1 >= 0 && j + j1 < m && field[i + i1][j + j1] == '#')
                            {
                                result.com = '!';
                                result.x = i + i1 + 1;
                                result.y = j + j1 + 1;
                                printf("%c %d %d\n", result.com, result.x, result.y);
                                sleep(1);
                                return result;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (field[i][j] == '#'){
                result.com = '!';
                result.x = i + 1;
                result.y = j + 1;
                printf("%c %d %d\n", result.com, result.x, result.y);
                sleep(1);
                return result;
            }
        }
    }

    return result;
}

#endif