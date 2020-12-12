#include "Scene.h"

class Story : public Scene {
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void ProcessInput() override;
	void Render(ShaderProgram* program) override;
};