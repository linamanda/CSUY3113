#include "Level3.h"

#define LEVEL3_WIDTH 10
#define LEVEL3_HEIGHT 24

#define LEVEL3_ENEMY_COUNT 2

unsigned int level3_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 3, 4, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 3, 3, 3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0, 3, 3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 3, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 3, 3, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

Level3::Level3(int* livesInp) {
    lives = livesInp;
}

void Level3::Initialize() {
    state.nextScene = -1;
    type = GAME;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 5, 1);

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
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint dogEnemyTextureID = Util::LoadTexture("dogEnemy.png");
    GLuint catEnemyTextureID = Util::LoadTexture("catEnemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = dogEnemyTextureID;
    state.enemies[0].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[0].position = glm::vec3(3, -13, 0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].height = 0.95f;
    state.enemies[0].width = 0.8f;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = true;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = catEnemyTextureID;
    state.enemies[1].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[1].position = glm::vec3(0, 0, 0);
    state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].height = 0.95f;
    state.enemies[1].width = 0.8f;
    state.enemies[1].aiType = WALKDOWNANDSPEEDUP;
    state.enemies[1].aiState = WALKING_RIGHT;
    state.enemies[1].isActive = true;

    // Initialize weapon
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->vertices = new float[12]{ -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f, -0.1f, -0.25f, 0.1f, 0.25f, -0.1f, 0.25f };
    state.weapon->Update(0, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, 0, state.map);
    state.weapon->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
    }

    if ((state.player->position.x >= 4.75) && (state.player->position.x <= 5) && (state.player->position.y >= -1)) {
        type = WIN;
    }
}

void Level3::ProcessInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (type == GAME) {
        if (keys[SDL_SCANCODE_M]) {
            for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
                if ((state.weapon->isActive == true) && state.weapon->CheckCollision(&state.enemies[i])) {
                    state.enemies[i].isActive = false;
                }
            }
        }
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < (LEVEL3_ENEMY_COUNT); i++)
        if (state.enemies[i].isActive && state.player->isActive) state.enemies[i].Render(program);

    if (state.player->isActive) {
        state.player->Render(program);
        state.weapon->Render(program);
    }
}