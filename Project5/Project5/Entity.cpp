#include "Entity.h"
#include "Map.h"

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

void Entity::CheckCollisionsY(Map* map) {
	// Probes for tiles
	glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
	glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
	glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

	glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
	glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
	glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

	float penetration_x = 0;
	float penetration_y = 0;

	if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}

	if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
}

void Entity::CheckCollisionsX(Map* map) {
	// Probes for tiles
	glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
	glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
		position.x += penetration_x;
		velocity.x = 0;
		collidedLeft = true;
	}
	if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
		position.x -= penetration_x;
		velocity.x = 0;
		collidedRight = true;
	}
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

void Entity::AIWalkDownAndSpeedUp(Entity* player) {
	if ((aiState == WALKING_LEFT) && (position.x > 0.0f))
		movement = glm::vec3(-1, 0, 0);
	else
		aiState = WALKING_RIGHT;

	if ((aiState == WALKING_RIGHT) && (position.x < 9.0f))
		movement = glm::vec3(1, 0, 0);
	else
		aiState = WALKING_LEFT;

	if ((glm::distance(position, player->position) < 3.0f)) {
		if ((player->position.x < position.x) && (position.x > 0.0f)) {
			movement = glm::vec3(-1, 0, 0);
			speed = 2.0f;
		}
		else if ((player->position.x > position.x) && (position.x < 10.0f)) {
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
	case WALKDOWNANDSPEEDUP:
		AIWalkDownAndSpeedUp(player);
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

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map) {
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
	CheckCollisionsY(map);
	CheckCollisionsY(objects, objectCount); // Fix if needed

	position.x += velocity.x * deltaTime; // Move on X
	CheckCollisionsX(map);
	CheckCollisionsX(objects, objectCount); // Fix if needed

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	if (entityType == WEAPON) {
		modelMatrix = glm::rotate(modelMatrix, -0.78f, glm::vec3(0, 0, 1.0));
	}
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