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

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Title.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music* music;
Mix_Chunk* bounce;

Scene* currentScene;
Scene* sceneList[4];

int lives;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Stargazer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    // Initialize audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadMUS("backgroundMusic.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);

    bounce = Mix_LoadWAV("bounce.wav");
    Mix_Volume(-1, MIX_MAX_VOLUME / 15);

    // Initialize lives
    lives = 3;

    // Set current scene
    sceneList[0] = new Title();
    sceneList[1] = new Level1(&lives);
    sceneList[2] = new Level2(&lives);
    sceneList[3] = new Level3(&lives);
    SwitchToScene(sceneList[0]);

    // Enable transparancy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ProcessInput() {
    if (currentScene->type != TITLE) {
        currentScene->state.player->movement = glm::vec3(0);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                if (currentScene->type == TITLE)
                    currentScene->state.nextScene = 1;
                break;
            case SDLK_SPACE:
                if (currentScene->type != TITLE && currentScene->state.player->collidedBottom) {
                    currentScene->state.player->jump = true;
                    Mix_PlayChannel(-1, bounce, 0);
                }
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (currentScene->type == GAME) {
        if (currentScene->type == GAME) {
            if (keys[SDL_SCANCODE_A]) {
                if(currentScene->state.player->position.x > 0)
                    currentScene->state.player->movement.x = -1.0f;
            }
            else if (keys[SDL_SCANCODE_D]) {
                if (currentScene->state.player->position.x < 9)
                    currentScene->state.player->movement.x = 1.0f;
            }

            if (glm::length(currentScene->state.player->movement) > 1.0f) {
                currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
            }
        }
    }

    currentScene->ProcessInput();
}

// Update on fixed timestep

#define FIXED_TIMESTEP 0.0166666f
// 60 frames per second

float lastTicks = 0.0f;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < accumulator) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP and not deltaTime
        // The scene will always update the same amount each time
        currentScene->Update(FIXED_TIMESTEP);

        if (currentScene->type == GAME) {
            if (currentScene->state.player->isActive == false && lives > 0) {
                lives -= 1;
                if (lives > 0)
                    SwitchToScene(currentScene);
            }
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    if (currentScene->type == GAME) {
        viewMatrix = glm::mat4(1.0f);

        if (currentScene->state.player->position.y > -19.75) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5, -currentScene->state.player->position.y, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5, 19.75, 0));
        }
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);
    if (currentScene->type != TITLE) {
        std::string livesStr = "Lives: " + std::to_string(lives);
        Util::DrawText(&program, Util::LoadTexture("font.png"), livesStr, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 0.75, currentScene->state.player->position.y + 0.75, 0));
    }

    if (currentScene->type == WIN) {
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Win!", 1.0f, -0.5f, glm::vec3(2.75, -1, 0));
    }
    else if (currentScene->type == LOSS) {
        Util::DrawText(&program, Util::LoadTexture("font.png"), "You Lose!", 1.0f, -0.5f, glm::vec3(2.5, currentScene->state.player->position.y, 0));
    }

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeChunk(bounce);
    Mix_FreeMusic(music);

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        
        if (currentScene->type == GAME && lives <= 0) currentScene->type = LOSS;

        Render();
    }

    Shutdown();

    return 0;
}