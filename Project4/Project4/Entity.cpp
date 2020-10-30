#include "Entity.h"

Entity::Entity() {
    modelMatrix = glm::mat4(1.0f);
    movement = glm::vec3(0);
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0.0f;
}

// Check if this Entity collides with another indicated entity
bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdiff = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydiff = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    return (xdiff < 0) && (ydiff < 0);
}

// Check if this Entity collides with any indicated entities in terms of the y-coordinate and re-adjust if needed
void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];

		if (CheckCollision(object)) {
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));

			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
			}
		}
	}
}

// Check if this Entity collides with any indicated entities in terms of the x-coordinate and re-adjust if needed
void Entity::CheckCollisionsX(Entity* objects, int objectCount) {
	for (int i = 0; i < objectCount; i++) {
		Entity* object = &objects[i];

		if (CheckCollision(object)) {
			float ydist = fabs(position.x - object->position.x);
			float penetrationX = fabs(ydist - (width / 2.0f) - (object->width / 2.0f));

			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
			}
		}
	}
}

//fix
void Entity::AIPatroller() {
	movement = glm::vec3(-1, 0, 0);
}

void Entity::AIJumper() {
	;
}

void Entity::AIWaitAndGo(Entity* player) {
	;
}

void Entity::AI(Entity* player) {
	switch (aiType) {
	case PATROLLER:
		AIPatroller();
		break;

	case JUMPER:
		AIJumper();
		break;

	case WAITANDGO:
		AIWaitAndGo(player);
		break;
	}
}

//fix
void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount) {
	if (isActive == false) return;

	// Resetting collision flags
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	if (entityType == ENEMY) {
		AI(player);
	}

	// Handling the jump
	if (jump) {
		jump = false;
		velocity.y += jumpPower;
	}

	// Movement
	velocity.x = movement.x * speed;
	velocity += acceleration * deltaTime;

	// Update position taking into account of collision
	position.y += velocity.y * deltaTime; // Move on Y
	CheckCollisionsY(objects, objectCount); // Fix if needed

	position.x += velocity.x * deltaTime; // Move on X
	CheckCollisionsX(objects, objectCount); // Fix if needed

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