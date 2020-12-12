#include "Scene.h"

class Level1 : public Scene {
public:
	int* spiesKilled;
	int* snakesKilled;

	Level1(int* spiesKilledInp, int* snakesKilledInp);
	void Initialize() override;
	void Update(float deltaTime) override;
	void ProcessInput() override;
	void Render(ShaderProgram* program) override;
};