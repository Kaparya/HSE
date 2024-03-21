#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Haffman.h"

int checkFile(FILE* fileName) {
    if (!fileName){
        printf("\n\n\n\nOPEN FILE ERROR\n\n\n\n");
        return 1;
    }
    return 0;
}

int userChoice() {
    printf("\n\n\n\nChoose what you want to do:\n");
    printf("0. Exit\n");
    printf("1. Compress the file\n");
    printf("2. Decompress the file\n");
    printf("Input the number (0/1/2): ");
    char* result = (char*) calloc(100, sizeof(char));
    scanf("%s", result);

    if (strcmp(result, "0") != 0 && strcmp(result, "1") != 0 && strcmp(result, "2") != 0) {
        free(result);
        return userChoice();
    }
    int help = result[0] - '0';

    free(result);
    return help;
};

void askFileName(char *fileName) {
    scanf("%s", fileName);
    printf("\n\n");
}

int askOutputName(unsigned char (* data), NODE* list, int left, long long size, clock_t* startTime1, long double* after) {
    char* fileName = (char*)calloc(100, sizeof(char));
    printf("Input the name of compressed file: ");
    scanf("%s", fileName);
    printf("\n");
    //memorizing new initial time
    *startTime1 = clock();

    FILE* output = fopen(fileName, "wb");
    if (checkFile(output)){
        free(fileName);
        return 1;
    }

    saveTree(list, left, &output);

    //Stats output
    printf("WRITING NEW FILE...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++) {
        printf("=");
    }
    printf("\\\n");
    long long cur = 1;

    //writing file
    for (long long i = 0; i < size; i++) {
        long long part = size / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }


        fputc(data[i], output);
    }

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");

    //memorizing file size
    fseek(output, 0L, SEEK_END);
    *after = (long double)ftell(output);

    free(fileName);
    fclose(output);
    return 0;
}