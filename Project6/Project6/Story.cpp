#include "Story.h"
#include "Util.h"

void Story::Initialize() {
    type = STORY;
    state.nextScene = -1;
    state.map = NULL;
    state.player = NULL;
    state.enemies = NULL;
}

void Story::Update(float deltaTime) {

}

void Story::ProcessInput() {

}

void Story::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font.png"), "Spies and snakes have", 0.75, -0.4, glm::vec3(-4.25f, 2.0f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "infiltrated your forest!", 0.75, -0.4, glm::vec3(-4.25f, 1.25f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Protect your land by", 0.75, -0.4, glm::vec3(-4.25f, 0.5f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "taking out 25 spies", 0.75, -0.4, glm::vec3(-4.25f, -0.25f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "and 10 snakes!", 0.75, -0.4, glm::vec3(-4.25f, -1.0f, 0.0f));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press SPACE to start your mission.", 0.50, -0.3, glm::vec3(-4.25f, -2.25f, 0.0f));
}