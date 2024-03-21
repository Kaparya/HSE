#ifndef BOT_H
#define BOT_H

#include "generation.h"

int bot();

int neighbours_b(int i, int j, const char **field, int n, int m);

int neighbours_f(int i, int j, const char **field, int n, int m);

MOVE bot_movement(int n, int m, const char **field);

MOVE bot_movement_f(int n, int m);

#endif