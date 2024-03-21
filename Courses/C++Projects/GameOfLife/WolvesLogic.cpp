#include <stdio.h>
#include <stdlib.h>

#include "FieldActions.h"

void putNewWolf(OBJECT** field, int* numberOf, int i, int j, int* kids) {
    i = (i + fieldHeight) % fieldHeight;
    j = (j + fieldWidth) % fieldWidth;

    if (*kids >= addLifeWolf || field[i][j].name == 3)
        return;

    if (field[i][j].name == 1) {
        field[i][j].isGrassUnder = 1;
        field[i][j].grassLife = field[i][j].life;
        field[i][j].grassLifeExpectancy = field[i][j].lifeExpectancy;
    }
    else if (field[i][j].name == 2)
        numberOf[2]--;

    field[i][j].name = 3;
    field[i][j].life = 0;
    field[i][j].lifeExpectancy = lifeExpectancyWolf;
    field[i][j].readyToMakeFamily = 0;
    (*kids)++;
}

void wolvesMovement (OBJECT** field, int* numberOf) {
    int queueSize = fieldWidth * fieldHeight + 2;
    PAIR* queue = (PAIR*) calloc(queueSize, sizeof(PAIR));
    int queueStart = 0, queueEnd = 0;

    int used[fieldHeight][fieldWidth] = {0};

    OBJECT** fieldNew = (OBJECT**) calloc(fieldHeight, sizeof(OBJECT*));
    for (int i = 0; i < fieldHeight; i++)
        fieldNew[i] = (OBJECT*) calloc(fieldWidth, sizeof(OBJECT));
    copyField(field, fieldNew);

    // WOLVES EAT BUNNIES IF THEY ARE CLOSE + MAKE NEW WOLVES + MOVE TO THE CLOSEST WOLF (ready)
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {

            if (field[i][j].name == 2) {

                if (field[(i - 1 + fieldHeight) % fieldHeight][j].name == 3 && !field[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily) {
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, (i - 1 + fieldHeight) % fieldHeight, j, i, j);
                    continue;
                }
                if (field[i][(j - 1 + fieldWidth) % fieldWidth].name == 3 && !field[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily) {
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, i, (j - 1 + fieldWidth) % fieldWidth, i, j);
                    continue;
                }
                if (field[(i + 1) % fieldHeight][j].name == 3 && !field[(i + 1) % fieldHeight][j].readyToMakeFamily) {
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, (i + 1) % fieldHeight, j, i, j);
                    continue;
                }
                if (field[i][(j + 1) % fieldWidth].name == 3 && !field[i][(j + 1) % fieldWidth].readyToMakeFamily) {
                    used[i][j] = 1;
                    moveCell(fieldNew, numberOf, i, (j + 1) % fieldWidth, i, j);
                    continue;
                }

                queue[queueEnd].first = i;
                queue[queueEnd].second = j;
                queue[queueEnd].goI = i;
                queue[queueEnd].goJ = j;
                queueEnd = (queueEnd + 1) % (queueSize);

            }
            else if (fieldNew[i][j].name == 3 && fieldNew[i][j].readyToMakeFamily) {
                if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name == 3 && fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily) {
                    fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].readyToMakeFamily = 0;
                    fieldNew[i][j].readyToMakeFamily = 0;

                    int kids = 0;
                    putNewWolf(fieldNew, numberOf, i - 2, j, &kids);
                    putNewWolf(fieldNew, numberOf, i - 1, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i - 1, j + 1, &kids);
                    putNewWolf(fieldNew, numberOf, i, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i, j + 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 1, j, &kids);
                    putNewWolf(fieldNew, numberOf, i - 2, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i - 2, j + 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 2, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 2, j + 1, &kids);
                    numberOf[3] += kids;

                    continue;
                }
                if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name == 3 && fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily) {
                    fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].readyToMakeFamily = 0;
                    fieldNew[i][j].readyToMakeFamily = 0;

                    int kids = 0;
                    putNewWolf(fieldNew, numberOf, i - 1, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i - 1, j, &kids);
                    putNewWolf(fieldNew, numberOf, i, j - 2, &kids);
                    putNewWolf(fieldNew, numberOf, i, j + 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 1, j - 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 1, j, &kids);
                    putNewWolf(fieldNew, numberOf, i - 1, j - 2, &kids);
                    putNewWolf(fieldNew, numberOf, i - 1, j + 1, &kids);
                    putNewWolf(fieldNew, numberOf, i + 1, j - 2, &kids);
                    putNewWolf(fieldNew, numberOf, i + 1, j + 1, &kids);
                    numberOf[3] += kids;

                    continue;
                }

                PAIR* queueFam = (PAIR*) calloc(queueSize, sizeof(PAIR));
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

                    if (fieldNew[goI][goJ].name == 3 && fieldNew[goI][goJ].readyToMakeFamily && (i != goI || j != goJ ))
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

                if (0 < i - goI && i - goI <= fieldHeight / 2 || 0 < fieldHeight + i - goI && fieldHeight + i - goI <= fieldHeight / 2) {
                    if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name <= 1) {
                        moveCell(fieldNew, numberOf, i, j, (i - 1 + fieldHeight) % fieldHeight, j);
                    }
                    else if (0 < j - goJ && j - goJ <= fieldWidth / 2 || 0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    }
                    else {
                        if (fieldNew[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j + 1) % fieldWidth);
                        }
                    }
                }
                else if (goI == i) {
                    if (0 < j - goJ && j - goJ <= fieldWidth / 2 || 0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    }
                    else {
                        if (fieldNew[i][(j + 1) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j + 1) % fieldWidth);
                        }
                    }
                }
                else {
                    if (fieldNew[(i + 1) % fieldHeight][j].name <= 1) {
                        moveCell(fieldNew, numberOf, i, j, (i + 1) % fieldHeight, j);
                    }
                    else if (0 < j - goJ && j - goJ <= fieldWidth / 2 || 0 < fieldWidth + j - goJ && fieldWidth + j - goJ <= fieldWidth / 2) {
                        if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name <= 1) {
                            moveCell(fieldNew, numberOf, i, j, i, (j - 1 + fieldWidth) % fieldWidth);
                        }
                    }
                    else {
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

    while (queueEnd != queueStart) {
        int curI, curJ, goI, goJ;
        curI = queue[queueStart].first;
        curJ = queue[queueStart].second;
        goI = queue[queueStart].goI;
        goJ = queue[queueStart].goJ;
        queueStart = (queueStart + 1) % (queueSize);
        used[curI][curJ] = 1;

        if (field[curI][curJ].name == 3 && !field[curI][curJ].readyToMakeFamily) {

            if (0 < curI - goI && curI - goI <= fieldHeight / 2 || 0 < fieldHeight + curI - goI && fieldHeight + curI - goI <= fieldHeight / 2) {
                if (field[(curI - 1 + fieldHeight) % fieldHeight][curJ].name <= 1)
                    moveCell(field, numberOf, curI, curJ, (curI - 1 + fieldHeight) % fieldHeight, curJ);
                else if (0 < curJ - goJ && curJ - goJ <= fieldWidth / 2 || 0 < fieldWidth + curJ - goJ && fieldWidth + curJ - goJ <= fieldWidth / 2) {
                    if (field[curI][(curJ - 1 + fieldWidth) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ - 1 + fieldWidth) % fieldWidth);
                }
                else {
                    if (field[curI][(curJ + 1) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ + 1) % fieldWidth);
                }
            }
            else if (goI == curI) {
                if (0 < curJ - goJ && curJ - goJ <= fieldWidth / 2 || 0 < fieldWidth + curJ - goJ && fieldWidth + curJ - goJ <= fieldWidth / 2) {
                    if (field[curI][(curJ - 1 + fieldWidth) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ - 1 + fieldWidth) % fieldWidth);
                }
                else {
                    if (field[curI][(curJ + 1) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ + 1) % fieldWidth);
                }
            }
            else {
                if (field[(curI + 1) % fieldHeight][curJ].name <= 1)
                    moveCell(field, numberOf, curI, curJ, (curI + 1) % fieldHeight, curJ);
                else if (0 < curJ - goJ && curJ - goJ <= fieldWidth / 2 || 0 < fieldWidth + curJ - goJ && fieldWidth + curJ - goJ <= fieldWidth / 2) {
                    if (field[curI][(curJ - 1 + fieldWidth) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ - 1 + fieldWidth) % fieldWidth);
                }
                else {
                    if (field[curI][(curJ + 1) % fieldWidth].name <= 1)
                        moveCell(field, numberOf, curI, curJ, curI, (curJ + 1) % fieldWidth);
                }
            }

        }

        if (!used[(curI - 1 + fieldHeight) % fieldHeight][curJ]) {
            queue[queueEnd].goI = goI;
            queue[queueEnd].goJ = goJ;
            queue[queueEnd].first = (curI - 1 + fieldHeight) % fieldHeight;
            queue[queueEnd].second = curJ;
            used[(curI - 1 + fieldHeight) % fieldHeight][curJ] = 1;
            queueEnd = (queueEnd + 1) % (queueSize);
        }
        if (!used[curI][(curJ - 1 + fieldWidth) % fieldWidth]) {
            queue[queueEnd].goI = goI;
            queue[queueEnd].goJ = goJ;
            queue[queueEnd].first = curI;
            queue[queueEnd].second = (curJ - 1 + fieldWidth) % fieldWidth;
            used[curI][(curJ - 1 + fieldWidth) % fieldWidth] = 1;
            queueEnd = (queueEnd + 1) % (queueSize);
        }
        if (!used[(curI + 1) % fieldHeight][curJ]) {
            queue[queueEnd].goI = goI;
            queue[queueEnd].goJ = goJ;
            queue[queueEnd].first = (curI + 1) % fieldHeight;
            queue[queueEnd].second = curJ;
            used[(curI + 1) % fieldHeight][curJ] = 1;
            queueEnd = (queueEnd + 1) % (queueSize);
        }
        if (!used[curI][(curJ + 1) % fieldWidth]) {
            queue[queueEnd].goI = goI;
            queue[queueEnd].goJ = goJ;
            queue[queueEnd].first = curI;
            queue[queueEnd].second = (curJ + 1) % fieldWidth;
            used[curI][(curJ + 1) % fieldWidth] = 1;
            queueEnd = (queueEnd + 1) % (queueSize);
        }
    }

    free(queue);
    for (int i = 0; i < fieldHeight; i++)
        free(fieldNew[i]);
    free(fieldNew);
}