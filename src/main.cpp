#include "include/logic.h"
#include "include/types.h"
#include "include/globals.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>

Logic logic;

struct Animation
{
    bool isActive = false;
    std::string piece;
    Types::Coord start;
    Types::Coord end;
    sf::Clock clock;
    float duration = 0.25f; // Animation duration in seconds
} animation;

int turns = 1;
const int rows = 10;
const int cols = 11;
const int squareSize = 75;
bool isPieceSelected = false;
std::vector<Types::Coord> moveList;
std::vector<Types::Turn> turnHistory;
Types::Coord selectedSquare = {-1, -1};
std::string selectedPiece;

sf::Color colour1 = sf::Color(0xE5E5E5ff);
sf::Color colour2 = sf::Color(0x26403Cff);
sf::Color colourSelected = sf::Color(0x6290c8ff);
sf::Color colourMove = sf::Color(0xFBFF1255);

std::map<std::string, sf::Texture> textures;
std::map<std::string, sf::Sprite> images;

std::map<std::string, sf::Sprite> loadImages()
{
    std::vector<std::string> pieces = {"bKa", "wKa", "bK0", "wK0", "bK1", "wK1", "bAd", "wAd",
                                       "bVi", "wVi", "bGi", "wGi", "bTa", "wTa", "bMo", "wMo",
                                       "bRk", "wRk", "bEl", "wEl", "bCa", "wCa", "bWe", "wWe",
                                       "wp0", "wp1", "wpx", "wpK", "wpA", "wpV", "wpG", "wpT",
                                       "wpM", "wpR", "wpE", "wpC", "wpW", "bp0", "bp1", "bpx",
                                       "bpK", "bpA", "bpV", "bpG", "bpT", "bpM", "bpR", "bpE",
                                       "bpC", "bpW"};

    for (const auto &piece : pieces)
    {
        sf::Texture texture;
        if (!texture.loadFromFile("assets/pieces/" + piece + ".png"))
        {
            std::cerr << "Error loading image: " << piece << ".png" << std::endl;
            continue;
        }

        // Store the texture to keep it alive
        textures[piece] = texture;

        sf::Sprite sprite;
        sprite.setTexture(textures[piece]);

        // Resize the sprite to (squareSize, squareSize)
        sf::Vector2u textureSize = texture.getSize();
        sprite.setScale(
            static_cast<float>(squareSize) / textureSize.x,
            static_cast<float>(squareSize) / textureSize.y);

        images[piece] = sprite;
    }

    return images;
}

Types::Coord calculateSquare(int x, int y)
{
    int _x = x / squareSize - 1;
    int _y = y / squareSize;
    Types::Coord coord = {_x, _y};
    return coord;
}

bool clickInBoard(const int x, const int y)
{
    const int boardOffset = 75;
    const int boardWidth = 900;
    const int boardHeight = 750;
    if (x < boardOffset || x > boardWidth || y > boardHeight)
    {
        return false;
    }
    return true;
}

void startAnimation(std::string piece, Types::Coord start, Types::Coord end, float duration)
{
    animation.isActive = true;
    animation.piece = piece;
    animation.start = start;
    animation.end = end;
    animation.duration = duration;
    animation.clock.restart();
}

sf::Vector2f interpolate(sf::Vector2f startPos, sf::Vector2f endPos, float t)
{
    return startPos + t * (endPos - startPos);
}

void updateAnimations(float deltaTime)
{
    if (animation.isActive)
    {
        float elapsedTime = animation.clock.getElapsedTime().asSeconds();
        if (elapsedTime >= animation.duration)
        {
            animation.isActive = false;
        }
    }
}

void highlightSquare(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    // Position and draw the selected square
    square.setPosition((selectedSquare.x + 1) * squareSize, selectedSquare.y * squareSize);
    square.setFillColor(colourSelected);
    window.draw(square);

    // Position and draw the possible move squares
    for (const auto &coord : moveList)
    {
        square.setPosition((coord.x + 1) * squareSize, coord.y * squareSize);
        square.setFillColor(colourMove);
        window.draw(square);
    }
}

void drawBoard(sf::RenderWindow &window)
{
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            // Position the square
            square.setPosition((col + 1) * squareSize, row * squareSize);

            // Alternate colors
            if ((row + col) % 2 != 0)
            {
                square.setFillColor(colour1);
            }
            else
            {
                square.setFillColor(colour2);
            }
            // Draw the square
            window.draw(square);
        }
    }

    // Draw the fortresses (squares on the sides)
    square.setSize(sf::Vector2f(squareSize, squareSize));

    // Left Fortress
    square.setPosition(0, squareSize);
    square.setFillColor(colour2);
    window.draw(square);

    // Right Fortress
    square.setPosition(squareSize * 12, squareSize * 8);
    square.setFillColor(colour1);
    window.draw(square);
}

void drawPieces(sf::RenderWindow &window, auto images)
{
    auto boardState = chessboard.getBoardState();

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string piece = boardState[row][col];
            if (piece != "---")
            {
                sf::Sprite sprite = images[piece];
                if (animation.isActive && animation.piece == piece && animation.end.x == col && animation.end.y == row)
                {
                    float elapsedTime = animation.clock.getElapsedTime().asSeconds();
                    float t = elapsedTime / animation.duration;
                    sf::Vector2f startPos((animation.start.x + 1) * squareSize, animation.start.y * squareSize);
                    sf::Vector2f endPos((animation.end.x + 1) * squareSize, animation.end.y * squareSize);
                    sf::Vector2f currentPos = interpolate(startPos, endPos, t);
                    sprite.setPosition(currentPos);
                }
                else
                {
                    sprite.setPosition((col + 1) * squareSize, row * squareSize);
                }
                window.draw(sprite);
            }
        }
    }
}

sf::Sprite renderBackground(sf::RenderWindow &window, sf::Texture &backgroundTexture)
{
    // Load background texture from a file
    if (!backgroundTexture.loadFromFile("assets/wood.png"))
    {
        std::cerr << "Error loading background texture" << std::endl;
        throw;
    }

    // Create a sprite using the texture
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Get the size of the window and texture
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Calculate scale factors for x and y directions
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    // Apply the scale to the sprite
    backgroundSprite.setScale(scaleX, scaleY);
    return backgroundSprite;
}

void clickLogic(int x, int y)
{
    Types::Coord coord = calculateSquare(x, y);
    std::cout << coord.x << ", " << coord.y << " | " << chessboard.getPiece(coord) << std::endl;
    auto boardState = chessboard.getBoardState();
    std::string const selected = chessboard.getPiece(coord);
    char player = (turns % 2 == 0) ? 'b' : 'w'; // player turn is decided by even/odd (white goes on turn 1)

    // if reclicking on selected square, or on a non-valid square
    if (isPieceSelected)
    {
        for (const auto &move : moveList)
        {
            if (coord == move)
            {
                // Animate moving the piece
                startAnimation(selectedPiece, selectedSquare, move, 0.5f);
                std::string target = chessboard.getPiece(move);

                // Update the board state (but don't display yet)
                chessboard.setCell(selectedSquare, "---");
                chessboard.setCell(move, selectedPiece);

                // Check if the move puts the opponent's king in check
                auto newBoardState = chessboard.getBoardState();
                char enemy = (player == 'w') ? 'b' : 'w';
                if (logic.isKingInCheck(enemy))
                {
                    std::cout << "Check!" << std::endl;
                    // Optionally, you can add additional logic to visually indicate the check
                }

                Types::Turn newTurn = {
                    turns,
                    player,
                    selectedSquare,
                    move,
                    selectedPiece,
                    target};

                turnHistory.push_back(newTurn);
                turns++;
                isPieceSelected = false;
                moveList = {};
                selectedSquare = {-1, -1};
                break;
            }
        }
    }
    if ((selectedSquare == coord) || selected == "---")
    {
        isPieceSelected = false;
        moveList = {};
        selectedSquare = {-1, -1};
    }
    else if (selected[0] == player)
    {
        isPieceSelected = true;
        selectedSquare = {coord.x, coord.y};
        selectedPiece = chessboard.getPiece(selectedSquare);
        moveList = logic.getMoves(coord, selected, player);
    }
}

void undoLastMove()
{
    if (!turnHistory.empty())
    {
        // Get the last turn
        Types::Turn lastTurn = turnHistory.back();
        turnHistory.pop_back();

        // Revert the move
        chessboard.setCell(lastTurn.finalSquare, lastTurn.pieceCaptured);
        chessboard.setCell(lastTurn.initialSquare, lastTurn.pieceMoved);

        // Decrease the turn count
        turns--;

        // Optionally, update other game states such as the active player or any game status
        isPieceSelected = false;
        moveList = {};
        selectedSquare = {-1, -1};

        std::cout << "Undo move: " << lastTurn.pieceMoved << " from (" << lastTurn.finalSquare.x << ", " << lastTurn.finalSquare.y << ") to (" << lastTurn.initialSquare.x << ", " << lastTurn.initialSquare.y << ")" << std::endl;

        // Stop animation if it's still active
        animation.isActive = false;
    }
    else
    {
        std::cout << "No moves to undo" << std::endl;
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(975, 900), "Chessboard");

    // Declare the texture outside of the renderBackground function for persistence
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite = renderBackground(window, backgroundTexture);

    // Load piece images
    auto pieceImages = loadImages();

    sf::Clock deltaClock;

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse button pressed event
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bool boardClick = clickInBoard(event.mouseButton.x, event.mouseButton.y);
                    if (boardClick)
                    {
                        clickLogic(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }

            // Handle Ctrl+Z for undo
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.control && event.key.code == sf::Keyboard::Z)
                {
                    undoLastMove();
                }
            }
        }

        // Update animations
        updateAnimations(deltaTime);

        // Clear the window with white color
        window.clear(sf::Color::White);
        // Draw the background
        window.draw(backgroundSprite);
        // Draw the chessboard
        drawBoard(window);
        // highlight selected piece
        highlightSquare(window);
        // Draw the pieces
        drawPieces(window, pieceImages);

        // End the current frame
        window.display();
    }

    return 0;
}