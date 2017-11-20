#pragma once

#include <Core/GPU/Mesh.h>

namespace Sky {
	const float SKY_LENGTH = 100;
}

class SkyFactory
{
public:
	SkyFactory();
	~SkyFactory();
	static std::vector<VertexFormat> createSkyVertices();
};

