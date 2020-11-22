#include "Level2.h"

#define LEVEL2_WIDTH 10
#define LEVEL2_HEIGHT 24

#define LEVEL2_ENEMY_COUNT 1

unsigned int level2_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 2, 2, 4,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 2, 2, 2,
    0, 0, 2, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 0, 0, 0, 0, 0, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 2, 2, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0, 0, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

Level2::Level2(int* livesInp) {
    lives = livesInp;
}

void Level2::Initialize() {
    state.nextScene = -1;
    type = GAME;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 5, 1);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.player->position = glm::vec3(0, -20, 0);
    state.player->movement = glm::vec3(0);
    // The player is always in the state of free-fall
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    /*
    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;*/

    state.player->height = 1.0f;
    state.player->width = 0.7f;

    state.player->jumpPower = 6.0f;

    state.player->isActive = true;

    // Initialize Enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("catEnemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[0].position = glm::vec3(0, 0, 0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[0].speed = 1;

    state.enemies[0].height = 0.95f;
    state.enemies[0].width = 0.8f;

    state.enemies[0].aiType = WALKDOWNANDSPEEDUP;
    state.enemies[0].aiState = WALKING_RIGHT;

    state.enemies[0].isActive = true;

    // Initialize weapon
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->vertices = new float[12]{ -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f, -0.1f, -0.25f, 0.1f, 0.25f, -0.1f, 0.25f };
    state.weapon->Update(0, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, 0, state.map);
    state.weapon->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
    }

    if ((state.player->position.x >= 8.75) && (state.player->position.x <= 9) && (state.player->position.y >= -1)) {
        state.nextScene = 3;
    }
}

void Level2::ProcessInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (type == GAME) {
        if (keys[SDL_SCANCODE_M]) {
            for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
                if ((state.weapon->isActive == true) && state.weapon->CheckCollision(&state.enemies[i])) {
                    state.enemies[i].isActive = false;
                }
            }
        }
    }
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    
    for (int i = 0; i < (LEVEL2_ENEMY_COUNT); i++)
        if (state.enemies[i].isActive && state.player->isActive) state.enemies[i].Render(program);

    if (state.player->isActive) {
        state.player->Render(program);
        state.weapon->Render(program);
    }
}