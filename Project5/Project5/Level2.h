#include "Scene.h"

class Level2 : public Scene {
public:
	int* lives;

	Level2(int* lives);
	void Initialize() override;
	void Update(float deltaTime) override;
	void ProcessInput() override;
	void Render(ShaderProgram* program) override;
};