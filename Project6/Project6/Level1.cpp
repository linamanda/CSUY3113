#include "Level1.h"
#include "time.h"

#define LEVEL1_WIDTH 35
#define LEVEL1_HEIGHT 27

#define LEVEL1_SPY_COUNT 3
#define LEVEL1_SNAKE_COUNT 1

unsigned int level1_data[] =
{
    79, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 112, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 98,
    96, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 111, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 111, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 111, 97, 98,
    96, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 112, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 115
};


Level1::Level1(int* spiesKilledInp, int* snakesKilledInp) {
    spiesKilled = spiesKilledInp;
    snakesKilled = snakesKilledInp;
}

void Level1::Initialize() {
    state.nextScene = -1;
    type = GAME;

    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 17, 8);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.player->position = glm::vec3(1, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0);
    state.player->speed = 5.0f;
    state.player->textureID = Util::LoadTexture("queen.png");
    
    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animLeft;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 1.0f;
    state.player->width = 0.7f;

    state.player->jumpPower = 0.0f;

    state.player->isActive = true;

    // Initialize Enemies
    state.enemies = new Entity[LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT];
    GLuint spyTextureID = Util::LoadTexture("spy.png");
    GLuint snakeTextureID = Util::LoadTexture("snake.png");

    srand(time(0));

    for (int i = 0; i < LEVEL1_SPY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = spyTextureID;

        state.enemies[i].animRight = new int[9]{ 27, 28, 29, 30, 31, 32, 33, 34, 35 };
        state.enemies[i].animLeft = new int[9]{ 9, 10, 11, 12, 13, 14, 15, 16, 17 };
        state.enemies[i].animUp = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
        state.enemies[i].animDown = new int[9]{ 18, 19, 20, 21, 22, 23, 24, 25, 26 };

        state.enemies[i].animIndices = state.enemies[0].animRight;
        state.enemies[i].animFrames = 9;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 9;
        state.enemies[i].animRows = 4;

        state.enemies[i].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };

        do {
            state.enemies[i].position.x = rand() % 33 + 1;
            state.enemies[i].position.y = -(rand() % 25 + 1);
        } while ((state.enemies[i].position.x < state.player->position.x + 5) && (state.enemies[i].position.x > state.player->position.x - 5) && (-state.enemies[i].position.y < state.player->position.y + 5) && (-state.enemies[i].position.y > state.player->position.y - 5));

        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        
        if (i % 2 == 0)
            state.enemies[i].speed = 2.0f;
        else
            state.enemies[i].speed = 3.0f;

        state.enemies[i].height = 0.95f;
        state.enemies[i].width = 0.8f;

        state.enemies[i].aiType = WAITANDGO;
        state.enemies[i].aiState = IDLE;

        state.enemies[i].isActive = true;
    }

    for (int i = LEVEL1_SPY_COUNT; i < LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = snakeTextureID;

        state.enemies[i].animRight = new int[1]{ 1 };
        state.enemies[i].animLeft = new int[1]{ 0 };
        state.enemies[i].animUp = new int[1]{ 3 };
        state.enemies[i].animDown = new int[1]{ 2 };

        state.enemies[i].animIndices = state.enemies[0].animRight;
        state.enemies[i].animFrames = 1;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 1;
        state.enemies[i].animRows = 4;

        state.enemies[i].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };

        do {
            state.enemies[i].position.x = rand() % 33 + 1;
            state.enemies[i].position.y = -(rand() % 25 + 1);
        } while ((state.enemies[i].position.x < state.player->position.x + 5) && (state.enemies[i].position.x > state.player->position.x - 5) && (-state.enemies[i].position.y < state.player->position.y + 5) && (-state.enemies[i].position.y > state.player->position.y - 5));

        state.enemies[i].acceleration = glm::vec3(0, 0, 0);

        state.enemies[i].speed = 1.0f;

        state.enemies[i].height = 1.0f;
        state.enemies[i].width = 1.0f;

        state.enemies[i].aiType = PACEANDSPEEDUP;
        state.enemies[i].aiState = WALKINGLEFT;

        state.enemies[i].isActive = true;
    }
    
    // Initialize weapon
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->vertices = new float[12]{ -0.25f, -0.25f, 0.25f, -0.25f, 0.25f, 0.25f, -0.25f, -0.25f, 0.25f, 0.25f, -0.25f, 0.25f };
    state.weapon->Update(0, state.player, state.enemies, LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT, state.map);
}

void Level1::Update(float deltaTime) {
    if (type != WIN) {
        state.player->Update(deltaTime, state.player, NULL, 0, state.map);
        state.weapon->Update(deltaTime, state.player, state.enemies, LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT, state.map);

        srand(time(0));
        for (int i = 0; i < LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT; i++) {
            if (state.enemies[i].isActive == false) {
                do {
                    state.enemies[i].position.x = rand() % 33 + 1;
                    state.enemies[i].position.y = -(rand() % 25 + 1);
                } while ((state.enemies[i].position.x < state.player->position.x + 5) && (state.enemies[i].position.x > state.player->position.x - 5) && (-state.enemies[i].position.y < state.player->position.y + 5) && (-state.enemies[i].position.y > state.player->position.y - 5));

                state.enemies[i].isActive = true;
            }
            state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
        }
    }

    if ((*spiesKilled >= 25) && (*snakesKilled >= 10)) {
        type = WIN;
    }
}

void Level1::ProcessInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (type == GAME) {
        if (keys[SDL_SCANCODE_M]) {
            for (int i = 0; i < LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT; i++) {
                if ((state.weapon->isActive == true) && state.weapon->CheckCollision(&state.enemies[i])) {
                    state.enemies[i].isActive = false;
                    if (state.enemies[i].aiType == WAITANDGO) {
                        *spiesKilled += 1;
                    }
                    else if (state.enemies[i].aiType == PACEANDSPEEDUP) {
                        *snakesKilled += 1;
                    }
                }
            }
        }
    }
}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    
    for (int i = 0; i < LEVEL1_SPY_COUNT + LEVEL1_SNAKE_COUNT; i++)
        if (state.enemies[i].isActive && state.player->isActive) state.enemies[i].Render(program);
        
    if (state.player->isActive) {
        state.player->Render(program);
        state.weapon->Render(program);
    }
    else {
        type = LOSS;
    }
}