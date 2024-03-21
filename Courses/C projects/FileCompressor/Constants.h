#define FILESIZE (1024*1024*1024)

typedef struct Node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned int freq;
    struct Node* left, * right, * next;
} NODE;

typedef union Bit2char {
    unsigned char symb;
    struct bit {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    } mbit;
} BIT2CHAR;