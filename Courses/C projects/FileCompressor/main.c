#include <stdio.h>
#include "UserInteraction.h"
#include "Compressor.h"
#include "Decompressor.h"

void ExiT();

int main() {
    void (*a[3])() = {ExiT, compressFile, decompressFile};

    while (1) {
        int user = userChoice();
        a[user]();
        if (user == 0)
            break;
    }

}

void ExiT() {
    printf("\n\nTHE END\n\n");
}