#include "Entity.h"

Entity::Entity() {
    modelMatrix = glm::mat4(1.0f);
    movement = glm::vec3(0);
    position = glm::vec3(0);
    speed = 0.0f;
}

void Entity::Update(float deltaTime) {
    position += movement * speed * deltaTime;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}