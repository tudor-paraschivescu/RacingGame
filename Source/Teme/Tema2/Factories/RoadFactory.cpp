#include "RoadFactory.h"

int whiteBorders = 0;
int redBorders = 0;
const glm::vec3 WHITE_BORDER_COLOR = { 0.8, 0.8, 0.8 };
const glm::vec3 RED_BORDER_COLOR = { 0.9, 0.1, 0.1 };

inline glm::vec3 getBorderColor() {
	if (whiteBorders != 2) {
		whiteBorders++;
		return WHITE_BORDER_COLOR;
	}
	else {
		if (redBorders != 2) {
			redBorders++;
			return RED_BORDER_COLOR;
		}
		else {
			whiteBorders = 1;
			redBorders = 0;
			return WHITE_BORDER_COLOR;
		}
	}
}

RoadFactory::RoadFactory()
{
}

RoadFactory::~RoadFactory()
{
}

std::vector<VertexFormat> RoadFactory::createRoadPartVertices(glm::vec3 bottomLeftCorner)
{
	// Calculate all the points of the road part
	glm::vec3 bottomRightCorner = bottomLeftCorner +
			glm::vec3(Road::ROADPART_LENGTH, 0, 0);
	glm::vec3 topLeftCorner = bottomLeftCorner +
			glm::vec3(0, 0, -Road::ROADPART_LENGTH);
	glm::vec3 topRightCorner = bottomLeftCorner +
			glm::vec3(Road::ROADPART_LENGTH, 0, -Road::ROADPART_LENGTH);

	// Create the vertices vector
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec4(bottomLeftCorner, 1.0), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec4(topLeftCorner, 1.0), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec4(topRightCorner, 1.0), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec4(bottomRightCorner, 1.0), glm::vec3(0, 0, 0))
	};

	return vertices;
}

std::vector<unsigned short> RoadFactory::createRoadPartIndices()
{
	// Create the indices vector
	std::vector<unsigned short> indices =
	{
		0, 2, 1,	// First triangle
		0, 3, 2		// Second triangle
	};

	return indices;
}

// Magic happens
std::vector<VertexFormat> RoadFactory::createBorderVertices(glm::vec3 bottomLeftCorner,
		Road::BorderType type)
{
	glm::vec3 color = getBorderColor();
	float x = bottomLeftCorner[0];
	float y = bottomLeftCorner[1];
	float z = bottomLeftCorner[2];
	
	if (type == Road::BorderType::LEFT) {
		return {
			VertexFormat(glm::vec3(x - Road::BORDER_THICKNESS, y, z), color),
			VertexFormat(glm::vec3(x, y, z), color),
			VertexFormat(glm::vec3(x - Road::BORDER_THICKNESS, y + Road::BORDER_HEIGHT, z), color),
			VertexFormat(glm::vec3(x, y + Road::BORDER_HEIGHT, z), color),
			VertexFormat(glm::vec3(x - Road::BORDER_THICKNESS, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x - Road::BORDER_THICKNESS, y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x,  y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color)
		};
	}
	else if (type == Road::BorderType::RIGHT) {
		return {
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH + Road::BORDER_THICKNESS, y, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y + Road::BORDER_HEIGHT, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH + Road::BORDER_THICKNESS, y + Road::BORDER_HEIGHT, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH + Road::BORDER_THICKNESS, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH + Road::BORDER_THICKNESS,  y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color)
		};
	}
	else if (type == Road::BorderType::TOP) {
		return {
			VertexFormat(glm::vec3(x, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x, y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH), color),
			VertexFormat(glm::vec3(x, y, z - Road::ROADPART_LENGTH - Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z - Road::ROADPART_LENGTH - Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x, y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH - Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH,  y + Road::BORDER_HEIGHT, z - Road::ROADPART_LENGTH - Road::BORDER_THICKNESS), color)
		};
	}
	else if (type == Road::BorderType::BOTTOM) {
		return {
			VertexFormat(glm::vec3(x, y, z + Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z + Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x, y + Road::BORDER_HEIGHT, z + Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y + Road::BORDER_HEIGHT, z + Road::BORDER_THICKNESS), color),
			VertexFormat(glm::vec3(x, y, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH, y, z), color),
			VertexFormat(glm::vec3(x, y + Road::BORDER_HEIGHT, z), color),
			VertexFormat(glm::vec3(x + Road::ROADPART_LENGTH,  y + Road::BORDER_HEIGHT, z), color)
		};
	}
}

std::vector<unsigned short> RoadFactory::createCuboidIndices()
{
	return {
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};
}

float RoadFactory::getBorderX1(glm::vec3 bottomLeftCorner, Road::BorderType type)
{
	float x = bottomLeftCorner[0];
	float z = bottomLeftCorner[2];

	if (type == Road::BorderType::LEFT) {
		return x;
	}
	else if (type == Road::BorderType::RIGHT) {
		return x + Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::TOP) {
		return x + Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::BOTTOM) {
		return x + Road::ROADPART_LENGTH;
	}
}

float RoadFactory::getBorderY1(glm::vec3 bottomLeftCorner, Road::BorderType type)
{
	float x = bottomLeftCorner[0];
	float z = bottomLeftCorner[2];

	if (type == Road::BorderType::LEFT) {
		return z;
	}
	else if (type == Road::BorderType::RIGHT) {
		return z;
	}
	else if (type == Road::BorderType::TOP) {
		return z - Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::BOTTOM) {
		return z;
	}
}

float RoadFactory::getBorderX2(glm::vec3 bottomLeftCorner, Road::BorderType type)
{
	float x = bottomLeftCorner[0];
	float z = bottomLeftCorner[2];

	if (type == Road::BorderType::LEFT) {
		return x;
	}
	else if (type == Road::BorderType::RIGHT) {
		return x + Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::TOP) {
		return x;
	}
	else if (type == Road::BorderType::BOTTOM) {
		return x;
	}
}

float RoadFactory::getBorderY2(glm::vec3 bottomLeftCorner, Road::BorderType type)
{
	float x = bottomLeftCorner[0];
	float z = bottomLeftCorner[2];

	if (type == Road::BorderType::LEFT) {
		return z - Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::RIGHT) {
		return z - Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::TOP) {
		return z - Road::ROADPART_LENGTH;
	}
	else if (type == Road::BorderType::BOTTOM) {
		return z;
	}
}

