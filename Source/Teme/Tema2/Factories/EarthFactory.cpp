#include "EarthFactory.h"

EarthFactory::EarthFactory()
{
}

EarthFactory::~EarthFactory()
{
}

std::vector<VertexFormat> EarthFactory::createEarthVertices()
{
	float length = Earth::EARTH_LENGTH;
	return {
		VertexFormat(glm::vec3(-length, -length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, -0.01, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -0.01, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, -length, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -length, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, -0.01, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -0.01, -length), glm::vec3(0, 0, 0))
	};
}

