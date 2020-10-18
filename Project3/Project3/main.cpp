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
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Entity* spaceship;
    Entity* landingPlatforms;
    Entity* walls;
};

GameState state;

enum GameMode { IN_GAME, GAME_OVER };
GameMode mode = IN_GAME;

#define MAX_WALLS 30
#define MAX_LANDING_PLATFORMS 2

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    // Initialize walls
    state.walls = new Entity[MAX_WALLS];
    GLuint wallTextureID = LoadTexture("platform.png");

    glm::vec3 currPosLeft = glm::vec3(-4.5f, -3.25f, 0.0f);
    glm::vec3 currPosBottom = glm::vec3(-3.5f, -3.25f, 0.0f);
    glm::vec3 currPosRight = glm::vec3(4.5f, -2.25f, 0.0f);

    glm::vec3 currPosPlat1 = glm::vec3(3.5f, 2.75f, 0.0f);
    glm::vec3 currPosPlat2 = glm::vec3(-1.25f, -0.5f, 0.0f);

    for (int i = 0; i < MAX_WALLS; i++) {
        state.walls[i].textureID = wallTextureID;
        state.walls[i].vertices = new float[] { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
        if (i <= 7) {
            state.walls[i].position = currPosLeft;
            currPosLeft += glm::vec3(0.0f, 1.0f, 0.0f);
        }
        else if (i <= 16) {
            state.walls[i].position = currPosBottom;
            currPosBottom += glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if (i <= 24) {
            state.walls[i].position = currPosRight;
            currPosRight += glm::vec3(0.0f, 1.0f, 0.0f);
        }
        else if (i <= 27) {
            state.walls[i].position = currPosPlat1;
            currPosPlat1 += glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else {
            state.walls[i].position = currPosPlat2;
            currPosPlat2 += glm::vec3(1.0f, 0.0f, 0.0f);
        }
        state.walls[i].Update(0, NULL, 0);
    }
    
    // Initialize landing platforms
    state.landingPlatforms = new Entity[MAX_LANDING_PLATFORMS];
    GLuint platformTextureID = LoadTexture("landingPlatform.png");

    glm::vec3 currPosPlat = glm::vec3(1.5f, -2.4f, 0.0f);

    for (int i = 0; i < MAX_LANDING_PLATFORMS; i++) {
        state.landingPlatforms[i].textureID = platformTextureID;
        state.landingPlatforms[i].vertices = new float[] { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
        state.landingPlatforms[i].position = currPosPlat;
        currPosPlat += glm::vec3(0.8f, 0.0f, 0.0f);
        state.landingPlatforms[i].height = -0.6f;
        state.landingPlatforms[i].Update(0, NULL, 0);
    }

    // Initialize spaceship
    state.spaceship = new Entity();
    GLuint spaceshipTextureID = LoadTexture("spaceship.png");

    state.spaceship->textureID = spaceshipTextureID;
    state.spaceship->vertices = new float[] { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    state.spaceship->position = glm::vec3(0.0f, 4.25f, 0.0f);
    state.spaceship->speed = 1.0f;
    state.spaceship->acceleration = glm::vec3(0.0f, -0.3f, 0.0f);
    state.spaceship->Update(0, NULL, 0);
}

void ProcessInput() {
    // Reset movement to 0
    state.spaceship->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (mode == IN_GAME) {
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
            state.spaceship->acceleration.x -= 0.01f;
        }
        else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
            state.spaceship->acceleration.x += 0.01f;
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
            state.spaceship->Update(FIXED_TIMESTEP, state.walls, MAX_WALLS);

            for (int i = 0; i < MAX_WALLS; i++) {
                if (state.spaceship->CheckCollision(&state.walls[i]))
                    mode = GAME_OVER;
            }

            deltaTime -= FIXED_TIMESTEP;
        }

        accumulator = deltaTime;

        break;
    case GAME_OVER:
        break;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render walls
    for (int i = 0; i < MAX_WALLS; i++)
        state.walls[i].Render(&program);

    // Render landing platforms
    for (int i = 0; i < MAX_LANDING_PLATFORMS; i++)
        state.landingPlatforms[i].Render(&program);

    // Render spaceship
    state.spaceship->Render(&program);

    // Render result string
    if ((state.spaceship->position.y <= -2.2f) && (state.spaceship->position.x >= 1.0f) && (state.spaceship->position.x <= 2.5f))
        DrawText(&program, LoadTexture("font.png"), "Mission Successful", 1.0, -0.5, glm::vec3(-4.0f, 0.0f, 0.0f));
    else if (mode == GAME_OVER)
        DrawText(&program, LoadTexture("font.png"), "Mission Failed", 1.0, -0.5, glm::vec3(-3.2f, 0.0f, 0.0f));

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