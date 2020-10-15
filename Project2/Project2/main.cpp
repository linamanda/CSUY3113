#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define NUM_PLAYERS 2

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Entity* ball;
    Entity* players;
};

GameState state;

enum GameMode { GAME, GAME_OVER };
GameMode mode = GAME_OVER;

Mix_Music* music;
Mix_Chunk* bounce;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    // Initialize audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    bounce = Mix_LoadWAV("bounce.wav");
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.2f);

    // Enable transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize the pong ball
    state.ball = new Entity();

    state.ball->vertices = new float[] { -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f };

    // Initialize player paddles
    state.players = new Entity[NUM_PLAYERS];

    state.players[0].vertices = new float[] { -0.2f, -1.0f, 0.2f, -1.0f, 0.2f, 1.0f, -0.2f, -1.0f, 0.2f, 1.0f, -0.2f, 1.0f };
    state.players[1].vertices = new float[] { -0.2f, -1.0f, 0.2f, -1.0f, 0.2f, 1.0f, -0.2f, -1.0f, 0.2f, 1.0f, -0.2f, 1.0f };

    state.players[0].position = glm::vec3(-4.8f, 0.0f, 0.0f);
    state.players[1].position = glm::vec3(4.8f, 0.0f, 0.0f);

    state.players[0].speed = 2.0f;
    state.players[1].speed = 2.0f;
}

float lastTicks = 0.0f;

void ProcessInput() {
    // Reset movement to 0
    state.players[0].movement = glm::vec3(0);
    state.players[1].movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE) {
                mode = GAME;
                state.ball->movement.x = 1.0f;
                state.ball->movement.y = 1.0f;
                state.ball->speed = 3.0f;
            }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (mode == GAME) {
        if (keys[SDL_SCANCODE_W] && (state.players[0].position.y <= 2.75f)) {
            state.players[0].movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_S] && (state.players[0].position.y >= -2.75f)) {
            state.players[0].movement.y = -1.0f;
        }

        if (keys[SDL_SCANCODE_UP] && (state.players[1].position.y <= 2.75f)) {
            state.players[1].movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_DOWN] && (state.players[1].position.y >= -2.75f)) {
            state.players[1].movement.y = -1.0f;
        }
    }
}

void Update() {
    // Calculate deltaTime
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    // Update position of player paddles
    state.players[0].Update(deltaTime);
    state.players[1].Update(deltaTime);

    // Update the position of pong ball
    state.ball->Update(deltaTime);

    float xdiff, ydiff;

    // collision detection between pong ball and player paddle 0
    xdiff = fabs(state.ball->position.x - state.players[0].position.x) - ((0.2 + 0.4) / 2.0f);
    ydiff = fabs(state.ball->position.y - state.players[0].position.y) - ((0.2 + 2) / 2.0f);

    if (xdiff < 0 && ydiff < 0) {
        state.ball->movement.x = 1;
        Mix_PlayChannel(-1, bounce, 0);
    }
    
    // collision detection between pong ball and player paddle 1
    xdiff = fabs(state.ball->position.x - state.players[1].position.x) - ((0.2 + 0.4) / 2.0f);
    ydiff = fabs(state.ball->position.y - state.players[1].position.y) - ((0.2 + 2) / 2.0f);

    if (xdiff < 0 && ydiff < 0) {
        state.ball->movement.x = -1;
        Mix_PlayChannel(-1, bounce, 0);
    }

    // collision detection between pong ball and top edge
    if (state.ball->position.y + 0.1 >= 3.75) {
        state.ball->movement.y = -1;
        Mix_PlayChannel(-1, bounce, 0);
    }

    // collision detection between pong ball and bottom edge
    if (state.ball->position.y - 0.1 <= -3.75) {
        state.ball->movement.y = 1;
        Mix_PlayChannel(-1, bounce, 0);
    }

    // collision detection between pong ball and right edge
    if (state.ball->position.x + 0.1 >= 5.0) {
        state.ball->movement = glm::vec3(0);
        mode = GAME_OVER;
    }

    // collision detection between pong ball and left edge
    if (state.ball->position.x - 0.1 <= -5.0) {
        state.ball->movement = glm::vec3(0);
        mode = GAME_OVER;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render pong ball
    state.ball->Render(&program);

    // Render player paddles
    state.players[0].Render(&program);
    state.players[1].Render(&program);

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