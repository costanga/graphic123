#include<SFML/Graphics.hpp>
using namespace sf;

#define WIDTH 600
#define HEIGHT 300

struct Position
{
    int x;
    int y;
};

----------------------------------------------------------------------------------------------------------------------------------

게임창의 높이는 300 넓이는 600으로 정의합니다. 이 position구조체는 후에 공룡과 나무의 위치를 저장하는 데 사용됩니다.

----------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Dinosaur Game. By BlockDMask");
    window.setFramerateLimit(60);

----------------------------------------------------------------------------------------------------------------------------------

게임창을 생성하고 프레임 속도를 60FPS로 제한합니다.

----------------------------------------------------------------------------------------------------------------------------------


    // Dino textures
    Texture t1, t2;
    if (!t1.loadFromFile("images/dino1.png") || !t2.loadFromFile("images/dino2.png")) {
        return -1; // Exit if texture loading fails
    }

    Sprite dinoArr[2];
    dinoArr[0] = Sprite(t1);
    dinoArr[1] = Sprite(t2);

----------------------------------------------------------------------------------------------------------------------------------

공룡 이미지를 불러와 텍스처와 스프라이트 배열에 저장합니다. 만약 불러와지지 않았다면 프로그램이 종료됩니다.

----------------------------------------------------------------------------------------------------------------------------------

    static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;

    Position dinoPos;
    dinoPos.x = 50;
    dinoPos.y = DINO_Y_BOTTOM;

----------------------------------------------------------------------------------------------------------------------------------

공룡의 초기 위치를 설정하는 부분으로 x값은 50픽셀로 지정하고 y는 점프 액션을 구현해야 하므로 변수로 지정했습니다.

----------------------------------------------------------------------------------------------------------------------------------

    int index = 0;
    float frame = 0.f;
    float frameSpeed = 0.4f;
    const int changeCount = 5;

    const int gravity = 4;
    bool isJumping = false;
    bool isBottom = true;

----------------------------------------------------------------------------------------------------------------------------------

애니메이션 인덱스, 프레임 속도, 중력등등 변수를 초기화하는 부분입니다.

----------------------------------------------------------------------------------------------------------------------------------

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

----------------------------------------------------------------------------------------------------------------------------------

나무 이미지를 불러와 텍스처와 스프라이트를 설정하고 초기 위치를 지정하는 부분입니다. 공룡과 마찬가지로 로드에 실패할 시 프로그램이 종료됩니다.

----------------------------------------------------------------------------------------------------------------------------------

    bool isGameOver = false;

    // Game Over texture
    Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("images/game_over.png")) {
        return -1; // Exit if texture loading fails
    }
    Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setPosition(WIDTH / 2 - gameOverTexture.getSize().x / 2, HEIGHT / 2 - gameOverTexture.getSize().y / 2);
    
----------------------------------------------------------------------------------------------------------------------------------

나무랑 공룡과 동일하게 게임오버 이미지를 불러와 스프라이트를 설정하고 위치를 설정합니다

----------------------------------------------------------------------------------------------------------------------------------
    
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

----------------------------------------------------------------------------------------------------------------------------------

이벤트 루프를 통해 창을 닫거나 게임 오버 후 재시작 키(ENTER)를 감지합니다.

----------------------------------------------------------------------------------------------------------------------------------

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
            
----------------------------------------------------------------------------------------------------------------------------------

공룔의 점프를 담당하는 부분으로 스페이스키가 감지되면 점프를 하여 공룡의 y축값을 변경시킵니다.

----------------------------------------------------------------------------------------------------------------------------------

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

----------------------------------------------------------------------------------------------------------------------------------

나무를 이동시키고 공룡과 충돌하는지 검사하는 부분입니다. 트리는 왼쪽 끝(width값)에서 생성되어 treeSpeed만큼 x값을 감소시키고 그럼 트리가 왼쪽으로 이동하는 것 처럼 보입니다. 그 후 만일 트리의 x값이 0이라면 다시 왼쪽끝에서 생성합니다.  그 후 공룡과 트리가 충돌하였는지 검사하여 만일 공룡과 트리가 충동했다면 isGameOver변수를 true로 바꾸어 줍니다

----------------------------------------------------------------------------------------------------------------------------------

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

----------------------------------------------------------------------------------------------------------------------------------

공룡과 트리를 게임 창에 그리고 만일 isGameOver변수가 true라면 게임오버를 시키는 변수입니다.

----------------------------------------------------------------------------------------------------------------------------------
