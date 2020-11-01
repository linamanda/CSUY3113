#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Entity* player;
    Entity* weapon;
    Entity* enemies;
    Entity* platforms;
};

GameState state;

enum GameMode { IN_GAME, WIN, LOSS };
GameMode mode = IN_GAME;

#define MAX_GROUND_PLAT 10
#define MAX_PLAT1 3
#define MAX_PLAT2 4

#define MAX_ENEMIES 3

int activeEnemies = 3;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.2f);

    // Enable transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize platforms
    state.platforms = new Entity[MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2];

    GLuint groundPlatTexID = Util::LoadTexture("ground.png");
    GLuint platformTexID = Util::LoadTexture("platform.png");

    glm::vec3 currGoundPos = glm::vec3(-4.5f, -3.25f, 0.0f);
    glm::vec3 currPlat1Pos = glm::vec3(-1.5f, -0.5f, 0.0f);
    glm::vec3 currPlat2Pos = glm::vec3(2.0f, 1.5f, 0.0f);

    for (int i = 0; i < MAX_GROUND_PLAT; ++i) {
        state.platforms[i].textureID = groundPlatTexID;
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
        state.platforms[i].position = currGoundPos;
        currGoundPos += glm::vec3(1.0f, 0.0f, 0.0f);
        state.platforms[i].Update(0, NULL, NULL, 0);
    }

    for (int i = 0; i < MAX_PLAT1; ++i) {
        state.platforms[i + MAX_GROUND_PLAT].textureID = platformTexID;
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i + MAX_GROUND_PLAT].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
        state.platforms[i + MAX_GROUND_PLAT].position = currPlat1Pos;
        currPlat1Pos += glm::vec3(1.0f, 0.0f, 0.0f);
        state.platforms[i + MAX_GROUND_PLAT].Update(0, NULL, NULL, 0);
    }

    for (int i = 0; i < MAX_PLAT2; ++i) {
        state.platforms[i + MAX_GROUND_PLAT + MAX_PLAT1].textureID = platformTexID;
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i + MAX_GROUND_PLAT + MAX_PLAT1].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
        state.platforms[i + MAX_GROUND_PLAT + MAX_PLAT1].position = currPlat2Pos;
        currPlat2Pos += glm::vec3(1.0f, 0.0f, 0.0f);
        state.platforms[i + MAX_GROUND_PLAT + MAX_PLAT1].Update(0, NULL, NULL, 0);
    }

    // Initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.player->position = glm::vec3(-3.75f, -1.0f, 0.0f);
    state.player->acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.player->speed = 2.0f;
    state.player->jumpPower = 8.0f;
    state.player->Update(0, NULL, NULL, 0);

    // Initialize enemies
    state.enemies = new Entity[MAX_ENEMIES];
    GLuint dogEnemyTexID = Util::LoadTexture("dogEnemy.png");
    GLuint catEnemyTexID = Util::LoadTexture("catEnemy.png");

    state.enemies[0].textureID = dogEnemyTexID;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = WALKING_LEFT;
    state.enemies[0].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[0].position = glm::vec3(-1.0f, 1.0f, 0.0f);
    state.enemies[0].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].Update(0, state.player, state.platforms, MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2);

    state.enemies[1].textureID = dogEnemyTexID;
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[1].position = glm::vec3(3.75f, -1.0f, 0.0f);
    state.enemies[1].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[1].speed = 1.0f;
    state.enemies[1].Update(0, state.player, state.platforms, MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2);

    state.enemies[2].textureID = dogEnemyTexID;
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = PATROLANDSPEEDUP;
    state.enemies[2].aiState = WALKING_LEFT;
    state.enemies[2].vertices = new float[12]{ -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.enemies[2].position = glm::vec3(3.75f, 1.75f, 0.0f);
    state.enemies[2].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[2].speed = 1.0f;
    state.enemies[2].jumpPower = 3.0f;
    state.enemies[2].Update(0, state.player, state.platforms, MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2);

    // Initialize sword
    state.weapon = new Entity();
    state.weapon->entityType = WEAPON;
    state.weapon->textureID = Util::LoadTexture("sword.png");
    state.weapon->vertices = new float[12]{ -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f, -0.1f, -0.25f, 0.1f, 0.25f, -0.1f, 0.25f };
    state.weapon->Update(0, state.player, state.enemies, MAX_ENEMIES);
}

void ProcessInput() {
    // Reset movement to 0
    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (state.player->collidedBottom) state.player->jump = true;
                break;
            }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (mode == IN_GAME) {
        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->movement.x = -1.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;
        }
        if (keys[SDL_SCANCODE_X]) {
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if ((state.weapon->isActive == true) && state.weapon->CheckCollision(&state.enemies[i])) {
                    state.enemies[i].isActive = false;
                    activeEnemies -= 1;
                }
            }
        }
    }
}

// Fixed timestep updating (60 fps)
#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    switch (mode) {
    case IN_GAME:
        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }
        
        while (deltaTime >= FIXED_TIMESTEP) {
            state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2);

            for (int i = 0; i < MAX_ENEMIES; i++) {
                state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2);
            }

            state.weapon->Update(FIXED_TIMESTEP, state.player, state.enemies, MAX_ENEMIES);

            deltaTime -= FIXED_TIMESTEP;
        }

        accumulator = deltaTime;

        break;
    case WIN:
        break;
    case LOSS:
        break;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render platforms
    for (int i = 0; i < (MAX_GROUND_PLAT + MAX_PLAT1 + MAX_PLAT2); i++) {
        state.platforms[i].Render(&program);
    }

    // Render enemies
    for (int i = 0; i < (MAX_ENEMIES); i++) {
        if(state.enemies[i].isActive) state.enemies[i].Render(&program);
    }

    // Render player
    if(state.player->isActive) state.player->Render(&program);

    // Render weapon
    if (state.player->isActive) state.weapon->Render(&program);

    if (mode == WIN) {
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Win!", 1.0, -0.5, glm::vec3(-1.25f, 0.0f, 0.0f));
    }
    else if (mode == LOSS) {
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Lose!", 1.0, -0.5, glm::vec3(-1.75f, 0.0f, 0.0f));
    }

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (activeEnemies == 0) {
            mode = WIN;
        }
        if (state.player->isActive == false) {
            mode = LOSS;
        }

        Render();
    }

    Shutdown();

    return 0;
}