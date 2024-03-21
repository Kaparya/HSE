#include <SFML/Graphics.hpp>

#include "Constants.h"

using namespace sf;

void copyField(OBJECT** from, OBJECT** to);

void makeField(OBJECT** field, const int* numberOf);

void printStart(RenderWindow* window, Sprite* grassSprite, Text* text, RectangleShape* button);
void printField(OBJECT** field, int* numberOf, long long moves, RenderWindow* window, Sprite* wolfSprite, Sprite* bunnySprite, Sprite* grassSprite, Sprite* grass1Sprite, Sprite* grass2Sprite, Sprite* bunnyReadySprite, Sprite* wolfReadySprite, Text* text);
void printEnd(OBJECT** field, int* numberOf, int result, long long moves, RenderWindow* window, Sprite* wolfSprite, Sprite* bunnySprite, Sprite* grassSprite, Sprite* grass1Sprite, Sprite* grass2Sprite, Sprite* bunnyReadySprite, Sprite* wolfReadySprite, Text* text, Font* font, RectangleShape* button);

void addYear(OBJECT** field);

void checkLifeExpectancy (OBJECT** field, int* numberOf);

void moveCell(OBJECT** field, int* numberOf, int fromI, int fromJ, int toI, int toJ);

void updateNumberOf(OBJECT** field, int* numberOf);