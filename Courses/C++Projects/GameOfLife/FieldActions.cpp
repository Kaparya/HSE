#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "Constants.h"

using namespace sf;

int flagRand = 1;

void copyField(OBJECT** from, OBJECT** to) {
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            to[i][j].name = from[i][j].name;
            to[i][j].life = from[i][j].life;
            to[i][j].lifeExpectancy = from[i][j].lifeExpectancy;
            to[i][j].isGrassUnder = from[i][j].isGrassUnder;
            to[i][j].readyToMakeFamily = from[i][j].readyToMakeFamily;
            if (from[i][j].isGrassUnder) {
                to[i][j].grassLifeExpectancy = from[i][j].grassLifeExpectancy;
                to[i][j].grassLife = from[i][j].grassLife;
            }
        }
    }
}

void makeField(OBJECT** field, const int* numberOf) {

    if (randomGen && flagRand) {
        srand(time(0));
        rand();
    }
    else if (flagRand) {
        srand(0);
        rand();
        rand();
    }
    flagRand = 0;

    // plant grass
    for (int i = 0; i < numberOf[1]; i++) {
        int x, y;
        x = rand() % fieldHeight;
        y = rand() % fieldWidth;
        if (field[x][y].name != 0) {
            i--;
            continue;
        }
        field[x][y].name = 1;
        field[x][y].life = 0;
        field[x][y].lifeExpectancy = lifeExpectancyGrass;
        field[x][y].isGrassUnder = 0;
    }

    // place bunnies
    for (int i = 0; i < numberOf[2]; i++) {
        int x, y;
        x = rand() % fieldHeight;
        y = rand() % fieldWidth;
        if (field[x][y].name != 0) {
            i--;
            continue;
        }
        field[x][y].name = 2;
        field[x][y].life = 0;
        field[x][y].lifeExpectancy = lifeExpectancyBunny;
        field[x][y].isGrassUnder = 0;
        field[x][y].readyToMakeFamily = 0;
    }

    // place wolves
    for (int i = 0; i < numberOf[3]; i++) {
        int x, y;
        x = rand() % fieldHeight;
        y = rand() % fieldWidth;
        if (field[x][y].name != 0) {
            i--;
            continue;
        }
        field[x][y].name = 3;
        field[x][y].life = 0;
        field[x][y].lifeExpectancy = lifeExpectancyWolf;
        field[x][y].isGrassUnder = 0;
        field[x][y].readyToMakeFamily = 0;
    }
}
void printStart(RenderWindow* window, Sprite* grassSprite, Text* text, RectangleShape* button) {

    (*window).clear(Color(45, 45,44));

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            grassSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
            (*window).draw(*grassSprite);
        }
    }

    RectangleShape GameStartRect(Vector2f(windowWidth / 3.0, windowHeight / 3.0));
    GameStartRect.setFillColor(Color(45, 45, 44));
    GameStartRect.setPosition((windowWidth - GameStartRect.getSize().x) / 2.0f, (windowHeight - GameStartRect.getSize().y) / 2.0f);
    window->draw(GameStartRect);

    text->setColor(Color(204, 0, 0));
    text->setCharacterSize(windowHeight / 8.0f);
    text->setString("FOREST");
    text->setPosition((windowWidth - text->getLocalBounds().width) / 2.0f, (windowHeight - GameStartRect.getSize().y) / 2.0f + 32);
    window->draw(*text);

    button->setFillColor(Color(200, 0, 0));
    button->setPosition((windowWidth - button->getSize().x) / 2.0f, (windowHeight) / 2.0f + 115 - button->getSize().y / 2.0f);
    window->draw(*button);

    text->setColor(Color(45, 45, 44));
    text->setCharacterSize(windowHeight / 32.0f);
    text->setString("START");
    text->setPosition((windowWidth - text->getLocalBounds().width) / 2.0f, (windowHeight) / 2.0f + 80);
    window->draw(*text);

    (*window).display();
}
void printField(OBJECT** field, int* numberOf, long long moves, RenderWindow* window, Sprite* wolfSprite, Sprite* bunnySprite, Sprite* grassSprite, Sprite* grass1Sprite, Sprite* grass2Sprite, Sprite* bunnyReadySprite, Sprite* wolfReadySprite, Text* text) {

    (*window).clear(Color(45, 45,44));

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            grass2Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
            (*window).draw(*grass2Sprite);
            if (field[i][j].isGrassUnder) {
                switch (field[i][j].grassLife) {
                    /*case 2:
                        grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grass1Sprite);
                        break;
                    case lifeExpectancyGrass:
                        grass2Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grass2Sprite);
                        break;*/
                    default:
                        grassSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grassSprite);
                }
            }

            switch (field[i][j].name) {
                case 0:
                    break;
                case 1:
                    switch (field[i][j].life) {
                        /*case 2:
                            grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grass1Sprite);
                            break;
                        case lifeExpectancyGrass:
                            grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grass1Sprite);
                            break;*/
                        default:
                            grassSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grassSprite);
                    }
                    break;
                case 2:
                    if (field[i][j].readyToMakeFamily) {
                        bunnyReadySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*bunnyReadySprite);
                    }
                    else {
                        bunnySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*bunnySprite);
                    }
                    break;
                case 3:
                    if (field[i][j].readyToMakeFamily) {
                        wolfReadySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*wolfReadySprite);
                    }
                    else {
                        wolfSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*wolfSprite);
                    }
                    break;
            }
        }
    }


    text->setColor(Color::Red);
    text->setCharacterSize(pixelPerObject);
    text->setString("Grass alive: " + std::to_string(numberOf[1]) +
                    "   Bunnies alive: " + std::to_string(numberOf[2]) +
                    "   Wolves alive: " + std::to_string(numberOf[3]) +
                    "   MOVES MADE: " + std::to_string(moves));
    text->setPosition(0, windowHeight);
    window->draw(*text);

    (*window).display();
}
void printEnd(OBJECT** field, int* numberOf, int result, long long moves, RenderWindow* window, Sprite* wolfSprite, Sprite* bunnySprite, Sprite* grassSprite, Sprite* grass1Sprite, Sprite* grass2Sprite, Sprite* bunnyReadySprite, Sprite* wolfReadySprite, Text* text, Font* font, RectangleShape* button) {

    (*window).clear(Color(45, 45,44));

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            grass2Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
            (*window).draw(*grass2Sprite);
            if (field[i][j].isGrassUnder) {
                switch (field[i][j].grassLife) {
                    /*case 2:
                        grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grass1Sprite);
                        break;
                    case lifeExpectancyGrass:
                        grass2Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grass2Sprite);
                        break;*/
                    default:
                        grassSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*grassSprite);
                }
            }

            switch (field[i][j].name) {
                case 0:
                    break;
                case 1:
                    switch (field[i][j].life) {
                        /*case 2:
                            grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grass1Sprite);
                            break;
                        case lifeExpectancyGrass:
                            grass1Sprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grass1Sprite);
                            break;*/
                        default:
                            grassSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                            (*window).draw(*grassSprite);
                    }
                    break;
                case 2:
                    if (field[i][j].readyToMakeFamily) {
                        bunnyReadySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*bunnyReadySprite);
                    }
                    else {
                        bunnySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*bunnySprite);
                    }
                    break;
                case 3:
                    if (field[i][j].readyToMakeFamily) {
                        wolfReadySprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*wolfReadySprite);
                    }
                    else {
                        wolfSprite->setPosition(j * pixelPerObject, i * pixelPerObject);
                        (*window).draw(*wolfSprite);
                    }
                    break;
            }
        }
    }

    text->setColor(Color::Red);
    text->setCharacterSize(pixelPerObject);
    text->setString("Grass alive: " + std::to_string(numberOf[1]) +
                   "   Bunnies alive: " + std::to_string(numberOf[2]) +
                   "   Wolves alive: " + std::to_string(numberOf[3]) +
                   "   MOVES MADE: " + std::to_string(moves));
    text->setPosition(0, windowHeight);
    window->draw(*text);

    RectangleShape GameEndRect(Vector2f(windowWidth / 3.0f, windowHeight / 3.0f));
    GameEndRect.setFillColor(Color(45, 45, 44));
    GameEndRect.setPosition((windowWidth - GameEndRect.getSize().x) / 2.0f, (windowHeight - GameEndRect.getSize().y) / 2.0f - 16);

    Text GameOverText("", *font, windowHeight / 16.0f);
    text->setColor(Color::White);
    GameOverText.setString("GAME OVER!\n");
    GameOverText.setPosition((windowWidth - GameOverText.getLocalBounds().width) / 2.0f,
                             GameEndRect.getGlobalBounds().height);

    Text GameStatsText("", *font, windowHeight / 32.0f);
    GameStatsText.setColor(Color(255, 204, 204));
    switch (result) {
        case 1:
            GameStatsText.setString("All wolves died...");
            break;
        case 2:
            GameStatsText.setString("All bunnies died...");
            break;
        case 3:
            GameStatsText.setString("All animals died...");
            break;
        case 4:
            GameStatsText.setString("There is no grass left...");
            break;
        case 5:
            GameStatsText.setString("Only bunnies left...");
            break;
        case 6:
            GameStatsText.setString("Only wolves left...");
            break;
        case 7:
            GameStatsText.setString("Everything died...");
    }
    GameStatsText.setPosition((windowWidth - GameStatsText.getLocalBounds().width) / 2.0f,
                              GameEndRect.getGlobalBounds().height + GameOverText.getGlobalBounds().height);

    window->draw(GameEndRect);
    window->draw(GameOverText);
    window->draw(GameStatsText);

    button->setFillColor(Color(200, 0, 0));
    button->setPosition((windowWidth - button->getSize().x) / 2.0f, (windowHeight) / 2.0f + 115 - button->getSize().y / 2.0f);
    window->draw(*button);

    text->setColor(Color(45, 45, 44));
    text->setCharacterSize(windowHeight / 33.0f);
    text->setString("RESTART");
    text->setPosition((windowWidth - text->getLocalBounds().width) / 2.0f, (windowHeight) / 2.0f + 80);
    window->draw(*text);

    (*window).display();
}

void addYear(OBJECT** field) {
    for (int i = 0; i < fieldHeight; i++)
        for (int j = 0; j < fieldWidth; j++)
            if (field[i][j].name != 0) {
                field[i][j].life++;
                if (field[i][j].isGrassUnder)
                    field[i][j].grassLife++;
                if (field[i][j].name == 2 && field[i][j].life % lifeExpectancyBunny == ageOfReproductionBunny)
                    field[i][j].readyToMakeFamily = 1;
                if (field[i][j].name == 3 && field[i][j].life % lifeExpectancyWolf == ageOfReproductionWolf)
                    field[i][j].readyToMakeFamily = 1;
            }
}

void checkLifeExpectancy (OBJECT** field, int* numberOf) {
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {

            int placeGrassUnder = 0;
            if (field[i][j].life == field[i][j].lifeExpectancy && field[i][j].name > 0) {
                numberOf[field[i][j].name]--;
                field[i][j].name = 0;
                if (field[i][j].isGrassUnder)
                    placeGrassUnder = 1;
            }
            if (field[i][j].isGrassUnder && field[i][j].grassLife == field[i][j].grassLifeExpectancy) {
                field[i][j].isGrassUnder = 0;
                numberOf[1]--;
                placeGrassUnder = 0;
            }

            if (placeGrassUnder) {
                field[i][j].name = 1;
                field[i][j].life = field[i][j].grassLife;
                field[i][j].lifeExpectancy = field[i][j].grassLifeExpectancy;
                field[i][j].isGrassUnder = 0;
            }
        }
    }
}

void moveCell(OBJECT** field, int* numberOf, int fromI, int fromJ, int toI, int toJ) {
    if (field[toI][toJ].name == 1 && field[fromI][fromJ].name == 2) {
        numberOf[1]--;
    }
    else if (field[toI][toJ].name == 1) {
        field[toI][toJ].isGrassUnder = 1;
        field[toI][toJ].grassLife = field[toI][toJ].life;
        field[toI][toJ].grassLifeExpectancy = field[toI][toJ].lifeExpectancy;
    }
    else if (field[toI][toJ].name == 2 && field[fromI][fromJ].name == 3) {
        numberOf[2]--;
        field[fromI][fromJ].lifeExpectancy += addLifeWolf;
    }

    field[toI][toJ].name = field[fromI][fromJ].name;
    field[fromI][fromJ].name = 0;
    field[toI][toJ].life = field[fromI][fromJ].life;
    field[fromI][fromJ].life = 0;
    field[toI][toJ].lifeExpectancy = field[fromI][fromJ].lifeExpectancy;
    field[fromI][fromJ].lifeExpectancy = 0;
    field[toI][toJ].readyToMakeFamily = field[fromI][fromJ].readyToMakeFamily;
    field[fromI][fromJ].readyToMakeFamily = 0;

    if (field[fromI][fromJ].isGrassUnder) {
        field[fromI][fromJ].name = 1;
        field[fromI][fromJ].life = field[fromI][fromJ].grassLife;
        field[fromI][fromJ].lifeExpectancy = field[fromI][fromJ].grassLifeExpectancy;
        field[fromI][fromJ].isGrassUnder = 0;
    }
}

void updateNumberOf(OBJECT** field, int* numberOf) {
    numberOf[1] = numberOf[2] = numberOf[3] = 0;

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            if (field[i][j].name)
                numberOf[field[i][j].name]++;
            if (field[i][j].isGrassUnder)
                numberOf[1]++;
        }
    }
}