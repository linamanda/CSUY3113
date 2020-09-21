#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, astronautMatrix, cloudOneMatrix, cloudTwoMatrix, moonMatrix;

GLuint astronautTexID, cloudTexID, moonTexID;

float astronautX = 0.0f, cloudOneX = 0.0f, cloudTwoX = 0.0f, moonRotation = 0.0f;

bool astronautMovingRight = true, cloudOneMovingRight = true, cloudTwoMovingRight = false;

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

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Reach for the Stars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
    astronautMatrix = glm::mat4(1.0f);
    cloudOneMatrix = glm::mat4(1.0f);
    cloudTwoMatrix = glm::mat4(1.0f);
    moonMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(0.078f, 0.008f, 0.18f, 1.0f);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    astronautTexID = LoadTexture("astronaut.png");
    cloudTexID = LoadTexture("cloud.png");
    moonTexID = LoadTexture("moon.png");
}

float lastTicks = 0.0f;

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void AstronautMove(float distancePerSec) {
    if ((astronautX < 3.5f) && astronautMovingRight) {
        astronautX += distancePerSec;
    }
    else if ((astronautX > -3.5f) && !astronautMovingRight) {
        astronautX -= distancePerSec;
    }

    // >= and <= to account for rounding errors
    if (astronautX >= 3.5f)
        astronautMovingRight = false;
    if (astronautX <= -3.5f)
        astronautMovingRight = true;

    astronautMatrix = glm::translate(astronautMatrix, glm::vec3(astronautX, 0.0f, 0.0f));
}

void CloudOneMove(float distancePerSec) {
    if ((cloudOneX < 1.5f) && cloudOneMovingRight) {
        cloudOneX += distancePerSec;
    }
    else if ((cloudOneX > -1.5f) && !cloudOneMovingRight) {
        cloudOneX -= distancePerSec;
    }

    if (cloudOneX >= 1.5f)
        cloudOneMovingRight = false;
    if (cloudOneX <= -1.5f)
        cloudOneMovingRight = true;

    cloudOneMatrix = glm::translate(cloudOneMatrix, glm::vec3(cloudOneX, 0.0f, 0.0f));
}

void CloudTwoMove(float distancePerSec) {
    if ((cloudTwoX < 1.5f) && cloudTwoMovingRight) {
        cloudTwoX += distancePerSec;
    }
    else if ((cloudTwoX > -1.5f) && !cloudTwoMovingRight) {
        cloudTwoX -= distancePerSec;
    }

    if (cloudTwoX >= 1.5f)
        cloudTwoMovingRight = false;
    if (cloudTwoX <= -1.5f)
        cloudTwoMovingRight = true;

    cloudTwoMatrix = glm::translate(cloudTwoMatrix, glm::vec3(cloudTwoX, 0.0f, 0.0f));
}

void MoonRotate(float degrees) {
    moonRotation += degrees;
    moonMatrix = glm::rotate(moonMatrix, glm::radians(moonRotation), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Update() {
    astronautMatrix = glm::mat4(1.0f);
    astronautMatrix = glm::translate(astronautMatrix, glm::vec3(0.0f, -3.0f, 0.0f));

    cloudOneMatrix = glm::mat4(1.0f);
    cloudOneMatrix = glm::translate(cloudOneMatrix, glm::vec3(-2.5f, -1.0f, 0.0f));

    cloudTwoMatrix = glm::mat4(1.0f);
    cloudTwoMatrix = glm::translate(cloudTwoMatrix, glm::vec3(2.5f, 1.0f, 0.0f));

    moonMatrix = glm::mat4(1.0f);
    moonMatrix = glm::translate(moonMatrix, glm::vec3(3.5f, 3.0f, 0.0f));

    // Calculate deltatime
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    AstronautMove(2.0f * deltaTime);
    CloudOneMove(1.0f * deltaTime);
    CloudTwoMove(1.0f * deltaTime);
    MoonRotate(-20.0f * deltaTime);
}

void DrawAstronaut() {
    program.SetModelMatrix(astronautMatrix);
    glBindTexture(GL_TEXTURE_2D, astronautTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawCloudOne() {
    program.SetModelMatrix(cloudOneMatrix);
    glBindTexture(GL_TEXTURE_2D, cloudTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawCloudTwo() {
    program.SetModelMatrix(cloudTwoMatrix);
    glBindTexture(GL_TEXTURE_2D, cloudTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawMoon() {
    program.SetModelMatrix(moonMatrix);
    glBindTexture(GL_TEXTURE_2D, moonTexID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    float texCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    DrawAstronaut();
    DrawCloudOne();
    DrawCloudTwo();
    DrawMoon();

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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
