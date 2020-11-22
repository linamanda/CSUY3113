#include "Scene.h"

class Level3 : public Scene {
public:
	int* lives;

	Level3(int* lives);
	void Initialize() override;
	void Update(float deltaTime) override;
	void ProcessInput() override;
	void Render(ShaderProgram* program) override;
};