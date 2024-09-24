#pragma once

#include "gameLogic.h"
#include "utility.h"
#include <SFML/Graphics.hpp>
#include "types.h"

class Render
{
    GameLogic *gameLogic; // Pointer to GameLogic
    Utility *utility;     // Pointer to Utility
public:
    static bool animationInProgress;
    static Types::Coord move;

    sf::Sprite renderBackground(sf::RenderWindow &window, sf::Texture &backgroundTexture);
    std::map<std::string, sf::Sprite> loadImages();
    void updateAnimations(float deltaTime);
    void drawBoard(sf::RenderWindow &window);
    void drawExitButton(sf::RenderWindow &window);
    void winScreen(sf::RenderWindow &window);
    void drawMenuScreen(sf::RenderWindow &window);
    void highlightSquares(sf::RenderWindow &window);
    void highlightPreviousMove(sf::RenderWindow &window);
    void highlightKing(sf::RenderWindow &window, Types::Coord kingPosition, bool isInCheck);
    void drawPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void drawCapturedPieces(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration);
    void gameHandler(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages);
    void gameFrame(sf::RenderWindow &window, const std::map<std::string, sf::Sprite> &pieceImages, sf::Sprite &backgroundSprite);

private:
    sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t);
    void highlightSquare(sf::RenderWindow &window, const Types::Coord &coord);
    std::string findAssetsPath(const std::string &filename);
    void tintScreen(sf::RenderWindow &window);
    void drawSlider(sf::RenderWindow &window, sf::Font &font);
};