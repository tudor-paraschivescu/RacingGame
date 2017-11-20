#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Factories/RoadFactory.h"

class Homework2 : public SimpleScene
{
public:
	Homework2();
	~Homework2();

	void Init() override;

	Mesh *CreateMesh(std::string name, glm::vec3 bottomLeftCorner, Road::BorderType borderType);

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	// Prefixes for all objects
	const std::string ROADPART_PREFIX = "ROADPART";
	const std::string BORDER_PREFIX = "BORDER";

	// Track Configuration Files Constants
	const std::string PATH_TO_CONFIG_FILE = "D:\\Tudor\\Desktop\\Teme-EGC\\Source\\Teme\\Tema2\\Track1.conf";
	const std::string STRAIGHT_LINE = "STRAIGHT_LINE";
	const std::string LEFT_LINE = "LEFT_LINE";
	const std::string RIGHT_LINE = "RIGHT_LINE";

	const float ROAD_START_X = 20;
	const float ROAD_START_Z = 24;
	const int ROADBLOCKS_TO_RENDER = 10;
};
