#pragma once

#include <Core/GPU/Mesh.h>

namespace Earth {
	const float EARTH_LENGTH = 25;
}

class EarthFactory
{
public:
	EarthFactory();
	~EarthFactory();
	static std::vector<VertexFormat> createEarthVertices();
};

