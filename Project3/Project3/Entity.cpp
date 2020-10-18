#include "Entity.h"

Entity::Entity() {
    modelMatrix = glm::mat4(1.0f);
    movement = glm::vec3(0);
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0.0f;
}

bool Entity::CheckCollision(Entity* other) {
    float xdiff = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydiff = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    return (xdiff < 0) && (ydiff < 0);
}

void Entity::Update(float deltaTime, Entity* platforms, int platformCount) {
    // If there is a collision, do not update the following downwards movement
    for (int i = 0; i < platformCount; ++i) {
        if (CheckCollision(&platforms[i])) return;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}