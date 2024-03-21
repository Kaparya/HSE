#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "UserInteraction.h"

void compressFile() {
    unsigned char* data = calloc(FILESIZE, sizeof(char));
    unsigned long long dataInd = 0;
    long double before;

    //memorize initial time
    clock_t startTime = clock();

    //ask for filename
    char* fileName = (char*) calloc(100, sizeof(char));
    printf("\n\n\n\nCOMPRESSOR\n");
    printf("Input your filename: ");
    askFileName(fileName);

    FILE* input = fopen(fileName, "rb");

    free(fileName);

    if (checkFile(input)) {
        free(data);
        return;
    }

    //counting the size of file
    fseek(input, 0L, SEEK_END);
    long long length = ftell(input);
    before = (long double)length;
    fseek(input, 0, SEEK_SET);
    unsigned int numberOfSymbols[256] = {0};

    //Stats output
    printf("FILE READING...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++)
        printf("=");
    printf("\\\n");
    long long cur = 1;

    //file reading
    for (long long i = 0; i < length; i++) {
        long long part = length / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        data[dataInd] = fgetc(input);
        numberOfSymbols[(unsigned char) data[dataInd++]]++;
    }
    data[dataInd] = '\0';
    fclose(input);

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");

    NODE* list = NULL;

    //list creating
    for (int i = 0; i < 256; i++) {
        if (numberOfSymbols[i] > 0) {
            NODE *new = (NODE *) malloc(sizeof(NODE));
            new->isSymb = 1;
            new->freq = numberOfSymbols[i];
            new->symb = (unsigned char) i;
            addToList(new, &list);
        }
    }

    //list to tree
    createTree(&list);

    //receiving the word for each symbol
    char* translate[256];
    char now[1000];
    now[0] = '\0';
    goTree(list, translate, now, 0);

    unsigned char* dataNew = calloc(FILESIZE, sizeof(char));
    unsigned long long dataNewInd = 0;

    //Stats output
    printf("WORKING WITH FILE...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++)
        printf("=");
    printf("\\\n");
    cur = 1;

    //rewriting the string with new words for each symbol
    for (long long i = 0; i < dataInd; i++){
        long long part = dataInd / 10;
        if (part == 0)
            part++;

        if (i == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        for (int j = 0; j < strlen(translate[data[i]]); j++){
            dataNew[dataNewInd++] = translate[data[i]][j];
        }
    }

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");


    int left = 0;
    //memorizing number of useless symbols
    while (dataNewInd % 8 != 0) {
        dataNew[dataNewInd++] = '0';
        left++;
    }
    dataNew[dataNewInd] = '\0';

    unsigned char* result = calloc(dataNewInd / 8 + 1, sizeof(char));
    long long z = 0;
    BIT2CHAR symb;

    //Stats output
    printf("FILE COMPRESSION...\n");
    printf("/");
    for (int i = 0; i < 10 - 2; i++)
        printf("=");
    printf("\\\n");
    cur = 1;

    //compressing string
    for (; z < dataNewInd / 8; z++){
        long long part = dataNewInd / 8 / 10;
        if (part == 0)
            part++;

        if (z == part * cur && cur <= 10) {
            cur++;
            putchar('#');
            fflush(stdout);
        }

        symb.mbit.b1 = dataNew[z * 8];
        symb.mbit.b2 = dataNew[z * 8 + 1];
        symb.mbit.b3 = dataNew[z * 8 + 2];
        symb.mbit.b4 = dataNew[z * 8 + 3];
        symb.mbit.b5 = dataNew[z * 8 + 4];
        symb.mbit.b6 = dataNew[z * 8 + 5];
        symb.mbit.b7 = dataNew[z * 8 + 6];
        symb.mbit.b8 = dataNew[z * 8 + 7];
        result[z] = symb.symb;
    }
    result[z] = '\0';

    //Stats output
    while (cur <= 10) {
        cur++;
        putchar('#');
        fflush(stdout);
    }
    printf("\n\n");

    //memorizing end time and waiting for user input
    long double after;
    clock_t endTime = clock();
    //memorizing new initial time
    clock_t startTime1 = 0;

    //asking output name + writing our string
    if (askOutputName(result, list, left, z, &startTime1, &after)){
        free(data);
        free(dataNew);
        free(result);
        freeTree(&list);
        for (int i = 0; i < 256; i++)
            if (numberOfSymbols[i])
                free(translate[i]);
        return;
    }

    //memorizing new end time
    clock_t endTime1 = clock();

    //final output
    long double timeSpent = (long double) (endTime - startTime + endTime1 - startTime1) / CLOCKS_PER_SEC;
    char type[] = "BYTES";
    if (before > 1024){
        before /= 1024;
        after /= 1024;
        strcpy(type, "KB");
        if (before > 1024){
            before /= 1024;
            after /= 1024;
            strcpy(type, "MB");
        }
    }
    printf("\n\n---------------------------\n");
    printf("FILE COMPRESSED CORRECTLY!\n");
    printf("From %.3Lf %s to %.3Lf %s in %.3Lf seconds\n", before, type, after, type, timeSpent);
    printf("---------------------------");

    //clearing memory
    free(data);
    free(dataNew);
    free(result);
    freeTree(&list);
    for (int i = 0; i < 256; i++)
        if (numberOfSymbols[i])
            free(translate[i]);
}
