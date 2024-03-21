#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#include "FieldActions.h"
#include "GrassActions.h"
#include "BunnyLogic.h"
#include "WolvesLogic.h"

using namespace sf;

// 0 - clear
// 1 - grass
// 2 - bunny
// 3 - wolf

int main() {

    RenderWindow window(VideoMode(windowWidth, windowHeight + pixelPerObject * 1.4), "FOREST");
    window.setFramerateLimit(framerateLimit);

    Sprite bunnySprite;
    Image bunnyImage;
    bunnyImage.loadFromFile("images/bunny1.png");
    Texture bunnyTexture;
    bunnyTexture.loadFromImage(bunnyImage);
    bunnySprite.setTexture(bunnyTexture);
    bunnySprite.setScale(
            pixelPerObject / bunnySprite.getLocalBounds().height,
            pixelPerObject / bunnySprite.getLocalBounds().width);

    Sprite bunnyReadySprite;
    Image bunnyReadyImage;
    bunnyReadyImage.loadFromFile("images/bunnyReady.png");
    Texture bunnyReadyTexture;
    bunnyReadyTexture.loadFromImage(bunnyReadyImage);
    bunnyReadySprite.setTexture(bunnyReadyTexture);
    bunnyReadySprite.setScale(
            pixelPerObject / bunnyReadySprite.getLocalBounds().height,
            pixelPerObject / bunnyReadySprite.getLocalBounds().width);

    Sprite wolfSprite;
    Image wolfImage;
    wolfImage.loadFromFile("images/wolf1.png");
    Texture wolfTexture;
    wolfTexture.loadFromImage(wolfImage);
    wolfSprite.setTexture(wolfTexture);
    wolfSprite.setScale(
            pixelPerObject / wolfSprite.getLocalBounds().height,
            pixelPerObject / wolfSprite.getLocalBounds().width);

    Sprite wolfReadySprite;
    Image wolfReadyImage;
    wolfReadyImage.loadFromFile("images/wolfReady.png");
    Texture wolfReadyTexture;
    wolfReadyTexture.loadFromImage(wolfReadyImage);
    wolfReadySprite.setTexture(wolfReadyTexture);
    wolfReadySprite.setScale(
            pixelPerObject / wolfReadySprite.getLocalBounds().height,
            pixelPerObject / wolfReadySprite.getLocalBounds().width);

    Sprite grassSprite;
    Image grassImage;
    grassImage.loadFromFile("images/grass.png");
    Texture grassTexture;
    grassTexture.loadFromImage(grassImage);
    grassSprite.setTexture(grassTexture);
    grassSprite.setScale(
            pixelPerObject / grassSprite.getLocalBounds().height,
            pixelPerObject / grassSprite.getLocalBounds().width);

    Sprite grass1Sprite;
    Image grass1Image;
    grass1Image.loadFromFile("images/grass1.jpg");
    Texture grass1Texture;
    grass1Texture.loadFromImage(grass1Image);
    grass1Sprite.setTexture(grass1Texture);
    grass1Sprite.setScale(
            pixelPerObject / grass1Sprite.getLocalBounds().height,
            pixelPerObject / grass1Sprite.getLocalBounds().width);

    Sprite grass2Sprite;
    Image grass2Image;
    grass2Image.loadFromFile("images/grass2.jpg");
    Texture grass2Texture;
    grass2Texture.loadFromImage(grass2Image);
    grass2Sprite.setTexture(grass2Texture);
    grass2Sprite.setScale(
            pixelPerObject / grass2Sprite.getLocalBounds().height,
            pixelPerObject / grass2Sprite.getLocalBounds().width);

    Font font;
    font.loadFromFile("font.ttf");
    Text text("", font, pixelPerObject);
    RectangleShape button;

    while (window.isOpen()) {

        long long moves = 0;

        OBJECT **field = (OBJECT **) calloc(fieldHeight, sizeof(OBJECT *));
        for (int i = 0; i < fieldHeight; i++) {
            field[i] = (OBJECT *) calloc(fieldWidth, sizeof(OBJECT));
            for (int j = 0; j < fieldWidth; j++) {
                field[i][j].name = 0;
                field[i][j].isGrassUnder = 0;
            }
        }

        button.setSize(Vector2f(windowWidth / 10.0, windowHeight / 10.0));
        printStart(&window, &grassSprite, &text, &button);

        short fl = 1;
        while (window.isOpen() && fl) {
            Event event;

            while (window.pollEvent(event) && fl) {
                if (event.type == Event::Closed) {
                    window.close();
                    for (int i = 0; i < fieldHeight; i++)
                        free(field[i]);
                    free(field);
                    return 0;
                } else if (event.type == Event::MouseButtonPressed &&
                           button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    button.setSize(Vector2f(windowWidth / 11.0, windowHeight / 11.0));
                    printStart(&window, &grassSprite, &text, &button);
                } else if (event.type == Event::MouseButtonReleased &&
                           button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    fl = 0;
                    button.setSize(Vector2f(windowWidth / 10.0, windowHeight / 10.0));
                }
            }
        }

        int numberOf[4] = {0, numberOfGrass, numberOfBunnies, numberOfWolves};

        makeField(field, numberOf);

        printField(field, numberOf, moves, &window, &wolfSprite, &bunnySprite, &grassSprite, &grass1Sprite,
                   &grass2Sprite, &bunnyReadySprite, &wolfReadySprite, &text);


        fl = 1;
        int result = 0;
        while (fl) {

            Event event;
            while (window.pollEvent(event) && fl) {
                if (event.type == Event::Closed) {
                    window.close();
                    fl = 0;
                }
            }

            result = 0;
            for (int i = 1; i <= 3; i++) {
                result = result * 2;
                if (!numberOf[i])
                    result++;
            }
            if (result) {
                printEnd(field, numberOf, result, moves, &window, &wolfSprite, &bunnySprite, &grassSprite,
                         &grass1Sprite, &grass2Sprite, &bunnyReadySprite, &wolfReadySprite, &text, &font, &button);
                printf("%d...\n\n\nGAME OVER!!!", result);
                break;
            }

            checkLifeExpectancy(field, numberOf);

            wolvesMovement(field, numberOf);
            bunniesMovement(field, numberOf);

            plantGrass(field, numberOf);

            addYear(field);

            printField(field, numberOf, moves, &window, &wolfSprite, &bunnySprite, &grassSprite, &grass1Sprite,
                       &grass2Sprite, &bunnyReadySprite, &wolfReadySprite, &text);

            updateNumberOf(field, numberOf);

            moves++;

        }

        fl = 1;
        while (window.isOpen() && fl) {
            Event event;

            while (window.pollEvent(event) && fl) {
                if (event.type == Event::Closed) {
                    window.close();

                    for (int i = 0; i < fieldHeight; i++)
                        free(field[i]);
                    free(field);

                    return 0;
                } else if (event.type == Event::MouseButtonPressed &&
                           button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    button.setSize(Vector2f(windowWidth / 11.0, windowHeight / 11.0));
                    printEnd(field, numberOf, result, moves, &window, &wolfSprite, &bunnySprite, &grassSprite,
                             &grass1Sprite, &grass2Sprite, &bunnyReadySprite, &wolfReadySprite, &text, &font, &button);
                } else if (event.type == Event::MouseButtonReleased &&
                           button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    fl = 0;
                    button.setSize(Vector2f(windowWidth / 10.0, windowHeight / 10.0));
                }
            }
        }


        for (int i = 0; i < fieldHeight; i++)
            free(field[i]);
        free(field);
    }
    window.close();
}
