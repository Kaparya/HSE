#ifndef GAME_H
#define GAME_H

#include "generation.h"

void print_field(const char **field, const int *bombs, const int *move, const int *n, const int *m);

MOVE movement(const int *n, const int *m);

void BOOM(char **field, const int **field_bomb, const int *n, const int *m, int BOT);

void WIN(int *moves);

int check_flag(const int **field_bomb, MOVE *now);

int check(const int **field_bomb, MOVE *now, char **field, int *closed, int *n, int *m);

#endif