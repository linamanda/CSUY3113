#include "Level1.h"
#include "time.h"

#define LEVEL1_WIDTH 30
#define LEVEL1_HEIGHT 20

#define LEVEL1_ENEMY_COUNT 3

unsigned int level1_data[] =
{
    79, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 98,
    113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 115
};

Level1::Level1(int* livesInp) {
    lives = livesInp;
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

    state.player->animIndices = state.player->animRight;
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
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("spy.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;

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

        state.enemies[i].height = 1.0f;
        state.enemies[i].width = 0.7f;

        state.enemies[i].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };

        do {
            srand(time(0));
            state.enemies[i].position.x = rand() % 28 + 1;
            state.enemies[i].position.y = -(rand() % 18 + 1);
        } while ((state.enemies[i].position.x < state.player->position.x + 5) && (state.enemies[i].position.x > state.player->position.x - 5) && (-state.enemies[i].position.y < state.player->position.y + 5) && (-state.enemies[i].position.y > state.player->position.y - 5));

        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].speed = (float)i;

        state.enemies[i].height = 0.95f;
        state.enemies[i].width = 0.8f;

        state.enemies[i].aiType = WAITANDGO;
        state.enemies[i].aiState = IDLE;

        state.enemies[i].isActive = true;
    }
    /*
    // Initialize weapon
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->vertices = new float[12]{ -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f, -0.1f, -0.25f, 0.1f, 0.25f, -0.1f, 0.25f };
    state.weapon->Update(0, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);*/
}

int enemiesKilled = 0;

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, 0, state.map);
    //state.weapon->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
    }

    if (enemiesKilled == 15) {
        state.nextScene = 2;
    }
}

void Level1::ProcessInput() {
    /*
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (type == GAME) {
        if (keys[SDL_SCANCODE_M]) {
            for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
                if ((state.weapon->isActive == true) && state.weapon->CheckCollision(&state.enemies[i])) {
                    state.enemies[i].isActive = false;
                }
            }
        }
    }*/
}

void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
        if (state.enemies[i].isActive && state.player->isActive) state.enemies[i].Render(program);
        
    if (state.player->isActive) {
        state.player->Render(program);
        //state.weapon->Render(program);
    }
}