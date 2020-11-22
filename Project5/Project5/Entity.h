#pragma once
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
#include "Map.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, WEAPON };

enum AIType { PATROLLER, PATROLANDSPEEDUP, WAITANDGO };
enum AIState { IDLE, WALKING_LEFT, WALKING_RIGHT };

class Entity {
public:
	EntityType entityType;
	AIType aiType;
	AIState aiState;

	glm::mat4 modelMatrix;
	float* vertices;
	GLuint textureID;

	float width = 1.0f;
	float height = 1.0f;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	float speed;

	bool jump = false;
	float jumpPower = 0;

	bool isActive = true;

	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;

	Entity();

	bool CheckCollision(Entity* other);
	void CheckCollisionsY(Map* map);
	void CheckCollisionsX(Map* map);
	void CheckCollisionsY(Entity* objects, int objectCount);
	void CheckCollisionsX(Entity* objects, int objectCount);

	void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
	void Render(ShaderProgram* program);

	void AI(Entity* player);
	void AIPatroller();
	void AIPatrolAndSpeedUp(Entity* player);
	void AIWaitAndGo(Entity* player);

	void Weapon(Entity* player, Entity* enemies, int enemyCount);
};

/*
class Entity {
public:

	// Animation frames
	int* animRight = NULL;
	int* animLeft = NULL;
	int* animUp = NULL;
	int* animDown = NULL;
	// Animation frames array we will currently use
	int* animIndices = NULL;
	// Number of animation frames
	int animFrames = 0;
	// Current animation frame index
	int animIndex = 0;
	// Timer to change the frame index
	float animTime = 0;

	// Columns and Rows of texture atlas
	int animCols = 0;
	int animRows = 0;

	Entity();

	void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};*/