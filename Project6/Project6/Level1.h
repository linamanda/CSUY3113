#include "Scene.h"

class Level1 : public Scene {
public:
	int* lives;

	Level1(int* lives);
	void Initialize() override;
	void Update(float deltaTime) override;
	void ProcessInput() override;
	void Render(ShaderProgram* program) override;
};