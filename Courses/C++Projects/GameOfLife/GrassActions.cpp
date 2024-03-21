#include "FieldActions.h"
#include <stdlib.h>

void plantGrass (OBJECT** field, int* numberOf) {
    OBJECT** fieldNew = (OBJECT**) calloc(fieldHeight, sizeof(OBJECT*));
    for (int i = 0; i < fieldHeight; i++)
        fieldNew[i] = (OBJECT *) calloc(fieldWidth, sizeof(OBJECT));
    copyField(field, fieldNew);

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            if (field[i][j].name == 1 && field[i][j].life % 3 == 0) {
                    if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name == 0) {
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name = 1;
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].life = 0;
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].lifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    else if (fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].name == 3 && fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].isGrassUnder == 0) {
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].isGrassUnder = 1;
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].grassLife = 0;
                        fieldNew[(i - 1 + fieldHeight) % fieldHeight][j].grassLifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name == 0) {
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name = 1;
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].life = 0;
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].lifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    else if (fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].name == 3 && fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].isGrassUnder == 0) {
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].isGrassUnder = 1;
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].grassLife = 0;
                        fieldNew[i][(j - 1 + fieldWidth) % fieldWidth].grassLifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    if (fieldNew[(i + 1) % fieldHeight][j].name == 0) {
                        fieldNew[(i + 1) % fieldHeight][j].name = 1;
                        fieldNew[(i + 1) % fieldHeight][j].life = 0;
                        fieldNew[(i + 1) % fieldHeight][j].lifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    else if (fieldNew[(i + 1) % fieldHeight][j].name == 3 && fieldNew[(i + 1) % fieldHeight][j].isGrassUnder == 0) {
                        fieldNew[(i + 1) % fieldHeight][j].isGrassUnder = 1;
                        fieldNew[(i + 1) % fieldHeight][j].grassLife = 0;
                        fieldNew[(i + 1) % fieldHeight][j].grassLifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    if (fieldNew[i][(j + 1) % fieldWidth].name == 0) {
                        fieldNew[i][(j + 1) % fieldWidth].name = 1;
                        fieldNew[i][(j + 1) % fieldWidth].life = 0;
                        fieldNew[i][(j + 1) % fieldWidth].lifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
                    else if (fieldNew[i][(j + 1) % fieldWidth].name == 3 && fieldNew[i][(j + 1) % fieldWidth].isGrassUnder == 0) {
                        fieldNew[i][(j + 1) % fieldWidth].isGrassUnder = 1;
                        fieldNew[i][(j + 1) % fieldWidth].grassLife = 0;
                        fieldNew[i][(j + 1) % fieldWidth].grassLifeExpectancy = lifeExpectancyGrass;
                        numberOf[1]++;
                    }
            }
        }
    }
    copyField(fieldNew, field);
    for (int i = 0; i < fieldHeight; i++)
        free(fieldNew[i]);
    free(fieldNew);
}