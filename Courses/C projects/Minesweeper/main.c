#ifndef MAIN_C
#define MAIN_C

#include "generation.h"
#include "game.h"
#include "solution.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "bot.h"
#include "curses.h"

int main() {
    srand(time(0));
    rand();

    start();

    int difficulty = diff();
    int BOT = 0;
    if (difficulty == 4) {
        difficulty = bot();
        BOT = difficulty;
    }

    int n = width(&difficulty);
    int m = length(&difficulty);
    int bombs = boom(&difficulty);
    int closed = n * m;

    char **field = init_field(&n, &m);

    int move_number = 0;
    int **field_bomb = NULL;
    print_field(field, &bombs, &move_number, &n, &m);


    int flag_cor = 0, flag = 0;

    while (closed > bombs && (flag_cor != bombs || flag > flag_cor)) {
        move_number++;
        MOVE now;
        if (!BOT)
            now = movement(&n, &m);
        else if (move_number != 1) {
            now = bot_movement(n, m, field);
        } else {
            now = bot_movement_f(n, m);
        }
        if (now.com == 'q') {
            printf("\n\n\n\ngame finished\n\n\n\n");
            if (move_number > 1) {
                printf("\nDo you want to look at the solution? (1/0)");
                int a;
                if (!BOT)
                    scanf("%d", &a);
                else {
                    a = rand() % 2;
                    printf("\n%d\n\n", a);
                }
                if (a)
                    solving(field, field_bomb, &n, &m);
            }
            return 0;
        }
        now.x--;
        now.y--;
        if (move_number == 1)
            field_bomb = init_bombs(&bombs, &n, &m, &now);

        if (now.com == '!') {
            if (field[now.x][now.y] == 'F')
                flag--;
            if (check(field_bomb, &now, field, &closed, &n, &m)) {
                BOOM(field, field_bomb, &n, &m, BOT);
                return 0;
            }
        } else if (now.com == '?') {
            flag++;
            field[now.x][now.y] = 'F';
            flag_cor += check_flag(field_bomb, &now);
        } else {
            if (field[now.x][now.y] == 'F') {
                flag--;
                field[now.x][now.y] = '#';
            } else
                printf("There wasn't any flag in [x][y]");
        }
        bombs -= flag;
        print_field(field, &bombs, &move_number, &n, &m);
        bombs += flag;

    }

    WIN(&move_number);
    return 0;
}

#endif