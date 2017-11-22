#include "CarFactory.h"

CarFactory::CarFactory()
{
}

CarFactory::~CarFactory()
{
}

std::vector<VertexFormat> CarFactory::createCarVertices()
{
	float l = Car::CAR_LENGTH;
	float h = Car::CAR_HEIGHT;
	float w = Car::CAR_WIDTH;
	return {
		VertexFormat(glm::vec3(-w, 0, -l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(w, 0, -l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(-w, h, -l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(w, h, -l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(-w, 0, l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(w, 0, l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(-w, h, l), Car::CAR_COLOR),
		VertexFormat(glm::vec3(w, h, l), Car::CAR_COLOR)
	};
}
