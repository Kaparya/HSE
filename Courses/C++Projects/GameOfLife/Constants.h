
typedef struct Object {
    int name;
    int life;
    int lifeExpectancy;
    char isGrassUnder;
    int grassLife;
    int grassLifeExpectancy;
    char readyToMakeFamily;
} OBJECT;

typedef struct Pair {
    int first, second;
    int goI, goJ;
} PAIR;

const int windowWidth = 1440;
const int windowHeight = 900;

const int pixelPerObject = 16;

const int framerateLimit = 10;//10

const int randomGen = 0;

const int fieldWidth = windowWidth / pixelPerObject;
const int fieldHeight = windowHeight / pixelPerObject;
//================================
const int numberOfGrass = 40;
const int numberOfBunnies = 20;//20
const int numberOfWolves = 25;
//================================
const int ageOfReproductionBunny = 8;
const int ageOfReproductionWolf = 8;

const int lifeExpectancyGrass = 9;
const int lifeExpectancyBunny = 10;
const int lifeExpectancyWolf = 10;

const int addLifeWolf = 2;
const int addLifeBunny = 2;