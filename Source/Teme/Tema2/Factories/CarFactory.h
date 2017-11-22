#pragma once

#include <Core/GPU/Mesh.h>

namespace Car {
	const float CAR_LENGTH = 0.8;
	const float CAR_HEIGHT = 0.6;
	const float CAR_WIDTH = 0.4;
	const glm::vec3 CAR_COLOR(0.9, 0.9, 0.4);
}

class CarFactory
{
public:
	CarFactory();
	~CarFactory();
	static std::vector<VertexFormat> createCarVertices();
};

