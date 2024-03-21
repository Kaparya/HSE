#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "UserInteraction.h"

void decompressFile() {
    unsigned char* data = calloc(FILESIZE, sizeof(char));
    long long dataInd = 0;

    char* fileName = (char*) calloc(100, sizeof(char));
    printf("\n\n\n\nDECOMPRESSOR\n");
    printf("Input compressed file's name: ");
    askFileName(fileName);

    //memorizing initial time
    clock_t startTime = clock();

    FILE* input = fopen(fileName, "rb");
    if (checkFile(input)) {
        free(data);
        free(fileName);
        return;
    }

    //receiving file size
    fseek(input, 0L, SEEK_END);
    long long length = ftell(input);
    fseek(input, 0, SEEK_SET);

    //recovering tree
    NODE* tree = NULL;
    int left = 0;
    if (recoverTree(&tree, &left, &input)) {
        fclose(input);
        free(data);
        free(fileName);
        return;
    }
    char help1 = (char)fgetc(input);
    length -= ftell(input);
    if (help1 != '\n') {
        fclose(input);
        free(data);
        free(fileName);
        return;
    }
    NODE* now = tree;

    BIT2CHAR symb;

    //Stats output
    printf("FILE READING...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++) {
        printf("=");
    }
    printf("\\\n");
    long long cur = 1;

    //file reading + rewriting
    for (long long i = 0; i < length; i++) {
        long long part = length / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        unsigned char help = fgetc(input);
        symb.symb = help;
        data[dataInd++] = symb.mbit.b1;
        data[dataInd++] = symb.mbit.b2;
        data[dataInd++] = symb.mbit.b3;
        data[dataInd++] = symb.mbit.b4;
        data[dataInd++] = symb.mbit.b5;
        data[dataInd++] = symb.mbit.b6;
        data[dataInd++] = symb.mbit.b7;
        data[dataInd++] = symb.mbit.b8;
    }
    fclose(input);

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");

    unsigned char* dataNew = calloc(FILESIZE, sizeof(char));
    long long dataNewInd = 0;

    dataInd -= left;

    //Stats output
    printf("FILE DECOMPRESSION...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++) {
        printf("=");
    }
    printf("\\\n");
    cur = 1;

    //decompressing the file
    for (long long i = 0; i <= dataInd; i++){
        long long part = dataInd / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        if (now == NULL){
            printf("\n\n\n\nFILE DATA ERROR\n\n\n\n");
            free(data);
            free(fileName);
            free(dataNew);
            freeTree(&tree);
            return;
        }
        else if (now->isSymb){
            dataNew[dataNewInd++] = now->symb;
            now = tree;
        }

        if (data[i]) {
            now = now->right;
        }
        else {
            now = now->left;
        }
    }
    dataNew[dataNewInd] = '\0';

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");


    clock_t endTime = clock();

    printf("Input the name of new file: ");
    askFileName(fileName);
    FILE* output = fopen(fileName, "wb");
    if (checkFile(output)) {
        free(fileName);
        freeTree(&tree);
        free(data);
        free(dataNew);
        return;
    }

    clock_t startTime1 = clock();

    //Stats output
    printf("WRITING NEW FILE...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++) {
        printf("=");
    }
    printf("\\\n");
    cur = 1;

    //writing file
    for (long long i = 0; i < dataNewInd; i++) {

        long long part = dataNewInd / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        fputc(dataNew[i], output);
    }
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");

    clock_t endTime1 = clock();

    long double timeSpent = (long double)(endTime - startTime + endTime1 - startTime1) / CLOCKS_PER_SEC;
    printf("\n\n---------------------------\n");
    printf("FILE DECOMPRESSED CORRECTLY!\n");
    printf("In %.3Lf seconds\n", timeSpent);
    printf("---------------------------");

    free(fileName);
    freeTree(&tree);
    fclose(output);
    free(data);
    free(dataNew);
}