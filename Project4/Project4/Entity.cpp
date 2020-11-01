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

void Entity::AIPatroller() {
	if ((aiState == WALKING_LEFT) && (position.x > -1.75))
		movement = glm::vec3(-1, 0, 0);
	else
		aiState = WALKING_RIGHT;
	
	if ((aiState == WALKING_RIGHT) && (position.x < 0.75))
		movement = glm::vec3(1, 0, 0);
	else
		aiState = WALKING_LEFT;
}

void Entity::AIPatrolAndSpeedUp(Entity* player) {
	if ((aiState == WALKING_LEFT) && (position.x > 2.0f))
		movement = glm::vec3(-1, 0, 0);
	else
		aiState = WALKING_RIGHT;

	if ((aiState == WALKING_RIGHT) && (position.x < 4.5f))
		movement = glm::vec3(1, 0, 0);
	else
		aiState = WALKING_LEFT;
	
	if ((glm::distance(position, player->position) < 3.0f)) {
		if ((player->position.x < position.x) && (position.x > 2.0f)) {
			movement = glm::vec3(-1, 0, 0);
			speed = 2.0f;
		}
		else if ((player->position.x > position.x) && (position.x < 4.5f)) {
			movement = glm::vec3(1, 0, 0);
			speed = 2.0f;
		}
		else
			speed = 0.0f;
	}
	else
		speed = 1.0f;
}

void Entity::AIWaitAndGo(Entity* player) {
	switch (aiState) {
	case IDLE:
		if (glm::distance(position, player->position) < 3.0f)
			aiState = WALKING_LEFT;
		break;
	case WALKING_LEFT:
		if (player->position.x < position.x)
			movement = glm::vec3(-1, 0, 0);
		else
			aiState = WALKING_RIGHT;
		break;
	case WALKING_RIGHT:
		if (player->position.x > position.x)
			movement = glm::vec3(1, 0, 0);
		else
			aiState = WALKING_LEFT;
		break;
	}
}

void Entity::AI(Entity* player) {
	switch (aiType) {
	case PATROLLER:
		AIPatroller();
		break;

	case PATROLANDSPEEDUP:
		AIPatrolAndSpeedUp(player);
		break;

	case WAITANDGO:
		AIWaitAndGo(player);
		break;
	}

	if ((isActive == true) && CheckCollision(player)) {
		player->isActive = false;
	}
}

void Entity::Weapon(Entity* player, Entity* enemies, int enemyCount) {
	position = player->position + glm::vec3(0.4f, 0.0f, 0.0f);
}

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

	if (entityType == WEAPON) {
		Weapon(player, objects, objectCount);
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