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
	if (this == other) return false;

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

void Entity::AIWaitAndGo(Entity* player) {
	switch (aiState) {
	case IDLE:
		if (glm::distance(position, player->position) < 3.0f)
			aiState = WALKING;
		break;
	case WALKING:
		if (glm::distance(position.x, player->position.x) > glm::distance(position.y, player->position.y)) {
			if (player->position.x <= position.x) {
				movement = glm::vec3(-1, 0, 0);
				animIndices = animLeft;
			}
			if (player->position.x > position.x) {
				movement = glm::vec3(1, 0, 0);
				animIndices = animRight;
			}
		}
		else {
			if (player->position.y > position.y) {
				movement = glm::vec3(0, 1, 0);
				animIndices = animUp;
			}
			if (player->position.y <= position.y) {
				movement = glm::vec3(0, -1, 0);
				animIndices = animDown;
			}
		}
		break;
	}
}

void Entity::AIPaceAndSpeedUp(Entity* player) {
	if ((aiState == WALKINGLEFT) && (position.x > 0.0f)) {
		movement = glm::vec3(-1, 0, 0);
		animIndices = animLeft;
	}
	else
		aiState = WALKINGRIGHT;

	if ((aiState == WALKINGRIGHT) && (position.x < 9.0f)) {
		movement = glm::vec3(1, 0, 0);
		animIndices = animRight;
	}
	else
		aiState = WALKINGLEFT;

	if ((glm::distance(position, player->position) < 5.0f) && (glm::distance(position.x, player->position.x) > glm::distance(position.y, player->position.y))) {
		speed = 3.0f;
		if (player->position.x <= position.x) {
			movement = glm::vec3(-1, 0, 0);
			animIndices = animLeft;
			aiState = WALKINGLEFT;
		}
		if (player->position.x > position.x) {
			movement = glm::vec3(1, 0, 0);
			animIndices = animRight;
			aiState = WALKINGRIGHT;
		}
	}
	else if ((glm::distance(position, player->position) < 5.0f) && (glm::distance(position.x, player->position.x) < glm::distance(position.y, player->position.y))) {
		speed = 3.0f;
		if (player->position.y > position.y) {
			movement = glm::vec3(0, 1, 0);
			animIndices = animUp;
		}
		if (player->position.y <= position.y) {
			movement = glm::vec3(0, -1, 0);
			animIndices = animDown;
		}
	}
	else {
		speed = 1.0f;
		aiState = WALKINGLEFT;
	}
}

void Entity::AI(Entity* player) {
	switch (aiType) {
	case WAITANDGO:
		AIWaitAndGo(player);
		break;
	case PACEANDSPEEDUP:
		AIPaceAndSpeedUp(player);
		break;
	}

	if ((isActive == true) && CheckCollision(player)) {
		player->isActive = false;
	}
}

void Entity::Weapon(Entity* player, Entity* enemies, int enemyCount) {
	if (player->entityDirection == RIGHT) {
		position = player->position + glm::vec3(0.4f, 0.0f, 0.0f);
	}

	if (player->entityDirection == LEFT) {
		position = player->position + glm::vec3(-0.4f, 0.0f, 0.0f);
	}

	if (player->entityDirection == UP) {
		position = player->position + glm::vec3(0.0f, 0.4f, 0.0f);
	}

	if (player->entityDirection == DOWN) {
		position = player->position + glm::vec3(0.0f, -0.4f, 0.0f);
	}
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

	if (animIndices != NULL) {
		// We only want to update when we are moving (pressing/holding down a button)
		// If we're not moving, go to the first frame of the current direction animation
		if (glm::length(movement) != 0) {
			// Increasing animTime
			animTime += deltaTime;

			// Every quarter second, reset the timer and go to the next frame
			// If the frame is the last one in the list, start over from the beginning
			// The smaller the number for the animTime, the quicker the frame change
			if (animTime >= 0.25f) {
				animTime = 0.0f;
				animIndex++;
				if (animIndex >= animFrames) {
					animIndex = 0;
				}
			}
		}
		else {
			animIndex = 0;
		}
	}

	// Handling the jump
	if (jump) {
		jump = false;
		velocity.y += jumpPower;
	}

	// Movement
	velocity.x = movement.x * speed;
	velocity.y = movement.y * speed;
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
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
	float u = (float)(index % animCols) / (float)animCols;
	float v = (float)(index / animCols) / (float)animRows;

	float width = 1.0f / (float)animCols;
	float height = 1.0f / (float)animRows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v, u, v + height, u + width, v, u, v };

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
	program->SetModelMatrix(modelMatrix);

	// If we are trying to animate a sprite from a texture atlas, do so.
	if (animIndices != NULL) {
		DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
		return;
	}

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