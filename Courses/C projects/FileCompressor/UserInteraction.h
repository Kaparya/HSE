#include <time.h>
#include "Haffman.h"

int userChoice();

void askFileName(char* fileName);
int askOutputName(unsigned char (* data), NODE* list, int left, long long size, clock_t* startTime1, long double* after);

int checkFile(FILE* fileName);