#include<SFML/Graphics.hpp>
using namespace sf;

#define WIDTH 600
#define HEIGHT 300

struct Position
{
    int x;
    int y;
};

int main(void)
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Dinosaur Game. By BlockDMask");
    window.setFramerateLimit(60);

    // Dino textures
    Texture t1, t2;
    if (!t1.loadFromFile("images/dino1.png") || !t2.loadFromFile("images/dino2.png")) {
        return -1; // Exit if texture loading fails
    }

    Sprite dinoArr[2];
    dinoArr[0] = Sprite(t1);
    dinoArr[1] = Sprite(t2);

    static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;

    Position dinoPos;
    dinoPos.x = 50;
    dinoPos.y = DINO_Y_BOTTOM;

    int index = 0;
    float frame = 0.f;
    float frameSpeed = 0.4f;
    const int changeCount = 5;

    const int gravity = 4;
    bool isJumping = false;
    bool isBottom = true;

    // Tree texture
    Texture t3;
    if (!t3.loadFromFile("images/tree.png")) {
        return -1; // Exit if texture loading fails
    }

    Sprite tree(t3);

    static const int TREE_Y_BOTTOM = HEIGHT - t3.getSize().y;
    Position treePos;
    treePos.x = WIDTH - 20;
    treePos.y = TREE_Y_BOTTOM;

    const int treeSpeed = 8;

    bool isGameOver = false;

    // Game Over texture
    Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("images/game_over.png")) {
        return -1; // Exit if texture loading fails
    }
    Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setPosition(WIDTH / 2 - gameOverTexture.getSize().x / 2, HEIGHT / 2 - gameOverTexture.getSize().y / 2);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            // Restart the game when Enter is pressed after game over
            if (isGameOver && e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
            {
                isGameOver = false;
                dinoPos.x = 50;
                dinoPos.y = DINO_Y_BOTTOM;
                treePos.x = WIDTH - 20;
            }
        }

        if (!isGameOver)
        {
            // Logic

            // Dino jump
            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (isBottom && !isJumping)
                {
                    // Make jumping stage
                    isJumping = true;
                    isBottom = false;
                }
            }

            // Dino jump (up and down)
            if (isJumping)
            {
                dinoPos.y -= gravity;
            }
            else
            {
                dinoPos.y += gravity;
            }

            // Dino jump limit, dino bottom limit
            if (dinoPos.y >= DINO_Y_BOTTOM)
            {
                dinoPos.y = DINO_Y_BOTTOM;
                isBottom = true;
            }
            if (dinoPos.y <= DINO_Y_BOTTOM - 100)
            {
                isJumping = false;
            }

            // Dino step
            frame += frameSpeed;
            if (frame > changeCount)
            {
                frame -= changeCount;
                ++index;
                if (index >= 2) { index = 0; }
            }

            // Tree move
            if (treePos.x <= 0)
            {
                treePos.x = WIDTH;
            }
            else
            {
                treePos.x -= treeSpeed;
            }

            // Tree Position
            tree.setPosition(treePos.x, treePos.y);

            // Dino Position
            dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

            // Check for collision
            if (dinoArr[index].getGlobalBounds().intersects(tree.getGlobalBounds()))
            {
                isGameOver = true;
            }
        }

        // Draw
        window.clear(Color::White);
        window.draw(tree);
        if (!isGameOver)
        {
            window.draw(dinoArr[index]);
        }
        else
        {
            window.draw(gameOverSprite);
        }
        window.display();
    }
    return 0;
}
