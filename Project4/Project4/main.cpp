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
    Entity* enemies;
    Entity* platforms;
};

GameState state;

enum GameMode { IN_GAME, WIN, LOSS };
GameMode mode = IN_GAME;

#define MAX_GROUND_PLAT 10
#define MAX_PLAT1 3
#define MAX_PLAT2 4

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
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
            state.player->movement.x = -1.0f;
        }
        else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;
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

    // Render player
    state.player->Render(&program);

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
        Render();
    }

    Shutdown();

    return 0;
}