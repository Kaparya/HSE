#include <stdio.h>
#include <stdlib.h>

#include "FieldActions.h"

void putNewBunny(OBJECT **field, int *numberOf, int i, int j, int *kids) {
    i = (i + fieldHeight) % fieldHeight;
    j = (j + fieldWidth) % fieldWidth;

    if (*kids >= addLifeBunny || field[i][j].name >= 2)
        return;

    if (field[i][j].name == 1) {
        numberOf[1]--;
    }

    field[i][j].name = 2;
    field[i][j].life = 0;
    field[i][j].lifeExpectancy = lifeExpectancyBunny;
    field[i][j].readyToMakeFamily = 0;
    (*kids)++;
}

void bunniesMovement(OBJECT **field, int *numberOf) {
    int queueSize = fieldWidth * fieldHeight + 2;
    PAIR *queue = (PAIR *) calloc(queueSize, sizeof(PAIR));
    int queueStart = 0, queueEnd = 0;

    int used[fieldHeight][fieldWidth] = {0};

    OBJECT **fieldNew = (OBJECT **) calloc(fieldHeight, sizeof(OBJECT *));
    for (int i = 0; i < fieldHeight; i++)
        fieldNew[i] = (OBJECT *) calloc(fieldWidth, sizeof(OBJECT));
    copyField(field, fieldNew);

    // BUNNIES EAT GRASS IF THEY ARE CLOSE
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {

            if (field[i][j].name == 1) {

                if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name == 2 &&
                    !used[(i - 1 + fieldHeight) % fieldHeight][j] &&
                    !field[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily) {
                    numberOf[1]--;
                    fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].lifeExpectancy += addLifeBunny;
                    fieldNew[i][j].name = 0;
                    fieldNew[i][j].isGrassUnder = 0;
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, (i - 1 + fieldHeight) % fieldHeight, j, i, j);
                    continue;
                }
                if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name == 2 &&
                    !used[i][(j - 1 + fieldWidth) % fieldWidth] &&
                    !field[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily) {
                    numberOf[1]--;
                    fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].lifeExpectancy += addLifeBunny;
                    fieldNew[i][j].name = 0;
                    fieldNew[i][j].isGrassUnder = 0;
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, i, (j - 1 + fieldWidth) % fieldWidth, i, j);
                    continue;
                }
                if (fieldNew[(i + 1) % fieldHeight][j].name == 2 && !used[(i + 1) % fieldHeight][j] &&
                    !field[(i + 1) % fieldHeight][j].readyToMakeFamily) {
                    numberOf[1]--;
                    fieldNew[(i + 1) % fieldHeight][j].lifeExpectancy += addLifeBunny;
                    fieldNew[i][j].name = 0;
                    fieldNew[i][j].isGrassUnder = 0;
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, (i + 1) % fieldHeight, j, i, j);
                    continue;
                }
                if (fieldNew[i][(j + 1) % fieldWidth].name == 2 && !used[i][(j + 1) % fieldWidth] &&
                    !field[i][(j + 1) % fieldWidth].readyToMakeFamily) {
                    numberOf[1]--;
                    fieldNew[i][(j + 1) % fieldWidth].lifeExpectancy += addLifeBunny;
                    fieldNew[i][j].name = 0;
                    fieldNew[i][j].isGrassUnder = 0;
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, i, (j + 1) % fieldWidth, i, j);
                    continue;
                }

            } else if (field[i][j].name == 2 && field[i][j].readyToMakeFamily) {
                if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name == 2 &&
                    fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily) {
                    fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily = 0;
                    fieldNew[i][j].readyToMakeFamily = 0;

                    int kids = 0;
                    putNewBunny(fieldNew, numberOf, i - 2, j, &kids);
                    putNewBunny(fieldNew, numberOf, i - 1, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i - 1, j + 1, &kids);
                    putNewBunny(fieldNew, numberOf, i, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i, j + 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 1, j, &kids);
                    putNewBunny(fieldNew, numberOf, i - 2, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i - 2, j + 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 2, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 2, j + 1, &kids);
                    numberOf[3] += kids;

                    continue;
                }
                if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name == 2 &&
                    fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily) {
                    fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily = 0;
                    fieldNew[i][j].readyToMakeFamily = 0;

                    int kids = 0;
                    putNewBunny(fieldNew, numberOf, i - 1, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i - 1, j, &kids);
                    putNewBunny(fieldNew, numberOf, i, j - 2, &kids);
                    putNewBunny(fieldNew, numberOf, i, j + 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 1, j - 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 1, j, &kids);
                    putNewBunny(fieldNew, numberOf, i - 1, j - 2, &kids);
                    putNewBunny(fieldNew, numberOf, i - 1, j + 1, &kids);
                    putNewBunny(fieldNew, numberOf, i + 1, j - 2, &kids);
                    putNewBunny(fieldNew, numberOf, i + 1, j + 1, &kids);
                    numberOf[3] += kids;

                    continue;
                }

                PAIR *queueFam = (PAIR *) calloc(queueSize, sizeof(PAIR));
                int queueFamStart = 0, queueFamEnd = 0;
                int usedFam[fieldHeight][fieldWidth] = {0};
                queueFam[queueFamEnd].first = i;
                queueFam[queueFamEnd].second = j;
                queueFamEnd = (queueFamEnd + 1) % queueSize;

                int goI, goJ;
                while (queueFamStart != queueFamEnd) {
                    goI = queueFam[queueFamStart].first;
                    goJ = queueFam[queueFamStart].second;
                    queueFamStart = (queueFamStart + 1) % queueSize;
                    usedFam[goI][goJ] = 1;

                    if (fieldNew[goI][goJ].name == 2 && fieldNew[goI][goJ].readyToMakeFamily && (i != goI || j != goJ))
                        break;

                    if (!usedFam[(goI - 1 + fieldHeight) % fieldHeight][goJ]) {
                        usedFam[(goI - 1 + fieldHeight) % fieldHeight][goJ] = 1;
                        queueFam[queueFamEnd].first = (goI - 1 + fieldHeight) % fieldHeight;
                        queueFam[queueFamEnd].second = goJ;
                        queueFamEnd = (queueFamEnd + 1) % (queueSize);
                    }
                    if (!usedFam[goI][(goJ - 1 + fieldWidth) % fieldWidth]) {
                        usedFam[goI][(goJ - 1 + fieldWidth) % fieldWidth] = 1;
                        queueFam[queueFamEnd].first = goI;
                        queueFam[queueFamEnd].second = (goJ - 1 + fieldWidth) % fieldWidth;
                        queueFamEnd = (queueFamEnd + 1) % (queueSize);
                    }
                    if (!usedFam[(goI + 1) % fieldHeight][goJ]) {
                        usedFam[(goI + 1) % fieldHeight][goJ] = 1;
                        queueFam[queueFamEnd].first = (goI + 1) % fieldHeight;
                        queueFam[queueFamEnd].second = goJ;
                        queueFamEnd = (queueFamEnd + 1) % (queueSize);
                    }
                    if (!usedFam[goI][(goJ + 1) % fieldWidth]) {
                        usedFam[goI][(goJ + 1) % fieldWidth] = 1;
                        queueFam[queueFamEnd].first = goI;
                        queueFam[queueFamEnd].second = (goJ + 1) % fieldWidth;
                        queueFamEnd = (queueFamEnd + 1) % (queueSize);
                    }

                }
                if (queueFamStart == queueFamEnd)
                    continue;

                if (0 < i - goI && i - goI <= fieldHeight / 2 ||
                    0 < fieldHeight + i - goI && fieldHeight + i - goI <= fieldHeight / 2) {
                    if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name <= 1) {
                        moveCell(fieldNew, numberOf, i, j, (i - 1 + fieldHeight) % fieldHeight, j);
                    } else if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                               0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    } else {
                        if (fieldNew[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j + 1) % fieldWidth);
                        }
                    }
                } else if (goI == i) {
                    if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                        0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    } else {
                        if (fieldNew[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j + 1) % fieldWidth);
                        }
                    }
                } else {
                    if (fieldNew[(i + 1) % fieldHeight][j].name <= 1) {
                        moveCell(fieldNew, numberOf, i, j, (i + 1) % fieldHeight, j);
                    } else if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                               0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    } else {
                        if (fieldNew[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j + 1) % fieldWidth);
                        }
                    }
                }

                free(queueFam);

            }
        }
    }
    copyField(fieldNew, field);

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            if (field[i][j].name == 2 && !used[i][j] && !field[i][j].readyToMakeFamily) {

                queueStart = queueEnd = 0;
                queue[queueEnd].first = i;
                queue[queueEnd].second = j;
                queueEnd = (queueEnd + 1) % queueSize;
                int used1[fieldHeight][fieldWidth] = {0};
                used1[i][j] = 1;

                int goI, goJ;
                while (queueEnd != queueStart) {
                    goI = queue[queueStart].first;
                    goJ = queue[queueStart].second;
                    queueStart = (queueStart + 1) % queueSize;

                    if (field[goI][goJ].name == 1)
                        break;

                    if (!used1[(goI - 1 + fieldHeight) % fieldHeight][goJ]) {
                        queue[queueEnd].first = (goI - 1 + fieldHeight) % fieldHeight;
                        queue[queueEnd].second = goJ;
                        used1[(goI - 1 + fieldHeight) % fieldHeight][goJ] = 1;
                        queueEnd = (queueEnd + 1) % (queueSize);
                    }
                    if (!used1[goI][(goJ - 1 + fieldWidth) % fieldWidth]) {
                        queue[queueEnd].first = goI;
                        queue[queueEnd].second = (goJ - 1 + fieldWidth) % fieldWidth;
                        used1[goI][(goJ - 1 + fieldWidth) % fieldWidth] = 1;
                        queueEnd = (queueEnd + 1) % (queueSize);
                    }
                    if (!used1[(goI + 1) % fieldHeight][goJ]) {
                        queue[queueEnd].first = (goI + 1) % fieldHeight;
                        queue[queueEnd].second = goJ;
                        used1[(goI + 1) % fieldHeight][goJ] = 1;
                        queueEnd = (queueEnd + 1) % (queueSize);
                    }
                    if (!used1[goI][(goJ + 1) % fieldWidth]) {
                        queue[queueEnd].first = goI;
                        queue[queueEnd].second = (goJ + 1) % fieldWidth;
                        used1[goI][(goJ + 1) % fieldWidth] = 1;
                        queueEnd = (queueEnd + 1) % (queueSize);
                    }
                }
                if (queueEnd == queueStart)
                    continue;

                if (0 < i - goI && i - goI <= fieldHeight / 2 ||
                    0 < fieldHeight + i - goI && fieldHeight + i - goI <= fieldHeight / 2) {
                    if (field[(i - 1 + fieldHeight) % fieldHeight][j].name <= 1) {
                        moveCell(field, numberOf, i, j, (i - 1 + fieldHeight) % fieldHeight, j);
                        used[(i - 1 + fieldHeight) % fieldHeight][j] = 1;
                    } else if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                               0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (field[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                            used[i][(j - 1 + fieldWidth) % fieldWidth] = 1;
                        }
                    } else {
                        if (field[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j + 1) % fieldWidth);
                            used[i][(j + 1) % fieldWidth] = 1;
                        }
                    }
                } else if (goI == i) {
                    if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                        0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (field[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                            used[i][(j - 1 + fieldWidth) % fieldWidth] = 1;
                        }
                    } else {
                        if (field[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j + 1) % fieldWidth);
                            used[i][(j + 1) % fieldWidth] = 1;
                        }
                    }
                } else {
                    if (field[(i + 1) % fieldHeight][j].name <= 1) {
                        moveCell(field, numberOf, i, j, (i + 1) % fieldHeight, j);
                        used[(i + 1) % fieldHeight][j] = 1;
                    } else if (0 < j - goJ && j - goJ <= fieldWidth / 2 ||
                               0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (field[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                            used[i][(j - 1 + fieldWidth) % fieldWidth] = 1;
                        }
                    } else {
                        if (field[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(field, numberOf, i, j, i, (j + 1) % fieldWidth);
                            used[i][(j + 1) % fieldWidth] = 1;
                        }
                    }
                }

            }
        }
    }

    free(queue);
    for (int i = 0; i < fieldHeight; i++)
        free(fieldNew[i]);
    free(fieldNew);
}