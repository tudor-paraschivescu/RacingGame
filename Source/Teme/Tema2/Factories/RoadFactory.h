#pragma once

#include <Core/GPU/Mesh.h>

namespace Road {
	const float ROADPART_LENGTH = 3;
	const float BORDER_THICKNESS = 0.5;
	const float BORDER_HEIGHT = 0.5;

	enum BorderType {LEFT, RIGHT, TOP, BOTTOM};
}

class RoadFactory
{
public:
	RoadFactory();
	~RoadFactory();
	static std::vector<VertexFormat> createRoadPartVertices(glm::vec3 bottomLeftCorner);
	static std::vector<unsigned short> createRoadPartIndices();
	static std::vector<VertexFormat> createBorderVertices(glm::vec3 bottomLeftCorner,
			Road::BorderType type);
	static std::vector<unsigned short> createCuboidIndices();
	static float getBorderX1(glm::vec3 bottomLeftCorner, Road::BorderType type);
	static float getBorderY1(glm::vec3 bottomLeftCorner, Road::BorderType type);
	static float getBorderX2(glm::vec3 bottomLeftCorner, Road::BorderType type);
	static float getBorderY2(glm::vec3 bottomLeftCorner, Road::BorderType type);
};

