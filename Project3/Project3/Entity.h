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

class Entity {
public:
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

	Entity();

	void Update(float deltaTime, Entity* platforms, int platformCount);
	void Render(ShaderProgram* program);
	bool CheckCollision(Entity* other);
};