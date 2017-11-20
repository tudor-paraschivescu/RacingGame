#include "SkyFactory.h"

SkyFactory::SkyFactory()
{
}

SkyFactory::~SkyFactory()
{
}

std::vector<VertexFormat> SkyFactory::createSkyVertices()
{
	float length = Sky::SKY_LENGTH;
	return {
		VertexFormat(glm::vec3(-length, -length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, length, length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, -length, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, -length, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-length, length, -length), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(length, length, -length), glm::vec3(0, 0, 0))
	};
}
