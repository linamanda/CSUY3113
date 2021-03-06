#include "Title.h"
#include "Util.h"

void Title::Initialize() {
    type = TITLE;
    state.nextScene = -1;
    state.map = NULL;
    state.player = NULL;
    state.enemies = NULL;
}

void Title::Update(float deltaTime) {

}

void Title::ProcessInput() {

}

void Title::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font.png"), "STARGAZER", 1.25, -0.25, glm::vec3(-4.0f, 0.5f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "PRESS ENTER TO START!", 0.5, -0.25, glm::vec3(-2.35f, -0.25f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Controls: WASD to move, SPACE to jump, M to attack", 0.25, -0.12, glm::vec3(-4.5f, -3.25f, 0.0f));
}