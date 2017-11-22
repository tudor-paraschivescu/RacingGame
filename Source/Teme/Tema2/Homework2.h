#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Factories/CarFactory.h"
#include "Factories/RoadFactory.h"
#include "Factories/EarthFactory.h"
#include "Factories/SkyFactory.h"
#include "Objects\CollisionCircle.h"

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
	int GetMeshSHID(Mesh *mesh);
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	// Prefixes and shader IDs for all objects
	const std::string ROADPART_PREFIX = "ROADPART";
	const int ROADPART_SHID = 0;
	const std::string BORDER_PREFIX = "BORDER";
	const int BORDER_SHID = 1;
	const std::string EARTH_PREFIX = "EARTH";
	const int EARTH_SHID = 2;
	const std::string SKY_PREFIX = "SKY";
	const int SKY_SHID = 3;
	const std::string OBSTACLE_PREFIX = "OBSTACLE";
	const std::string CAR_PREFIX = "CAR";
	const std::string TIRE_PREFIX = "TIRE";
	const char SHADER_NAME[18] = "SHADER_HOMEWORK_2";

	// Obstacle data
	const int COUNT_OBSTACLES = 10;
	const float OBSTACLE_COORDS[20] = { 13, 9.5, 9, 11.5, -1, 11.5, -6, 9.5, -4, 2.5,
		-1, 0.5, 7, 1.5, 7, -8.5, -5, -7.5, 0, -19.5 };
	const float OBSTACLE_RADIUS = 0.5;

	// Car data
	const int COUNT_CAR_PARTS = 10;
	const int COUNT_TIRES = 4;
	const int CAR_INDEX = 8;
	float carCoords[10] = { 21.9f, 22.75f, 21.9f, 21.25f, 21.1f, 22.75f, 21.1f, 21.25f, 21.5, 22 };
	const float SPEED = 4.5;
	const float ROTATION = 50;
	float tireRotation = 0;
	bool canMove = false;

	bool rotateLeft = false;
	bool rotateRight = false;
	const float MOVEMENT_ROTATION = 50;
	float angleOfMovement = 0;

	// Camera data
	float x = carCoords[CAR_INDEX];
	float y = 5;
	float z = carCoords[CAR_INDEX + 1] + 4;

	// Track Configuration Files Constants
	const std::string PATH_TO_CONFIG_FILE = "D:\\Tudor\\Desktop\\Teme-EGC\\Source\\Teme\\Tema2\\Track1.conf";
	const std::string STRAIGHT_LINE = "STRAIGHT_LINE";
	const std::string LEFT_LINE = "LEFT_LINE";
	const std::string RIGHT_LINE = "RIGHT_LINE";

	// Constants and coordinates for meshes
	const float ROAD_START_X = 20;
	const float ROAD_START_Z = 24;
	const int ROADBLOCKS_TO_RENDER = 10;
	const float ONE_MINUTE = 1.5;
	
};
