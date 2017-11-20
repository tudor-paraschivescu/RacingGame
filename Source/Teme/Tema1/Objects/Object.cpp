#include "Object.h"
#include "../Homework1.h"
#include "Line.h"

Mesh* Object::CreateAstronaut(std::string name,
								glm::vec3 center,
								float edgeLength)
{
	// Calculate the offset coordinates of the triangle corners
	float medianLength = (((float) sqrt(5)) * edgeLength) / 2;
	float xA = 0;
	float yA = medianLength * 2 / 3;
	float xB = -edgeLength / 2;
	float yB = -medianLength / 3;
	float xC = -xB;
	float yC = yB;

	// Create the vertices vector
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(xA, yA, 0), Color::ASTRONAUT_TOP),
		VertexFormat(center + glm::vec3(xB, yB, 0), Color::ASTRONAUT_BASE),
		VertexFormat(center + glm::vec3(xC, yC, 0), Color::ASTRONAUT_BASE)
	};

	// Create the indices vector
	std::vector<unsigned short> indices = { 0, 1, 2 };

	// Create the mesh for the astronaut
	Mesh* astronaut = new Mesh(name);
	astronaut->InitFromData(vertices, indices);

	return astronaut;
}

Mesh* Object::CreateAsteroid(std::string name,
						glm::vec3 center,
						float radius)
{
	// Create the vertices vector and add the center of the circle
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(center, Color::ASTEROID_CENTER));

	// Add DISCRETE_STEPS vertices from the arch of the circle to the vector
	float discreteAngularStep = 2 * ((float) M_PI) / Object::DISCRETE_STEPS;
	for (float angle = 0; angle <= 2 * M_PI; angle += discreteAngularStep) {
		glm::vec3 offset = glm::vec3(radius * cos(angle), radius * sin(angle), 0);
		vertices.push_back(VertexFormat(center + offset, Color::ASTEROID_ARCH));
	}

	// Add all the indices of the triangles to the vector
	std::vector<unsigned short> indices;
	for (int i = 1; i < Object::DISCRETE_STEPS; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	
	// Add the last triangle that has the last and first vertex
	indices.push_back(0);
	indices.push_back(Object::DISCRETE_STEPS);
	indices.push_back(1);

	// Create the mesh for the asteroid
	Mesh* asteroid = new Mesh(name);
	asteroid->InitFromData(vertices, indices);

	return asteroid;
}

Mesh* Object::CreatePlatform(std::string name,
						glm::vec3 bottomLeftCorner,
						float height,
						float width,
						PlatformType type)
{
	// Choose the color depending on the type of the platform
	glm::vec3 color;
	switch (type)
	{
	case Object::STATIONARY:
		color = Color::PLATFORM_STATIONARY;
		break;
	case Object::BOUNCE:
		color = Color::PLATFORM_BOUNCE;
		break;
	case Object::FINISH:
		color = Color::PLATFORM_FINISH;
		break;
	default:
		break;
	}

	glm::vec3 bottomRightCorner = bottomLeftCorner + glm::vec3(width, 0, 0);
	glm::vec3 topLeftCorner = bottomLeftCorner + glm::vec3(0, height, 0);
	glm::vec3 topRightCorner = bottomLeftCorner + glm::vec3(width, height, 0);

	// Create the vertices vector
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottomLeftCorner, color),
		VertexFormat(topLeftCorner, color),
		VertexFormat(topRightCorner, color),
		VertexFormat(bottomRightCorner, color)
	};

	// Create the indices vector
	std::vector<unsigned short> indices =
	{
		0, 2, 1,	// First triangle
		0, 3, 2		// Second triangle
	};

	// Create the mesh for the platform
	Mesh* platform = new Mesh(name);
	platform->InitFromData(vertices, indices);

	// Add collision lines
	Homework1::addCollisionLine(Line(bottomLeftCorner, bottomRightCorner, type, Line::BOTTOM));
	Homework1::addCollisionLine(Line(topLeftCorner, topRightCorner, type, Line::TOP));
	Homework1::addCollisionLine(Line(bottomLeftCorner, topLeftCorner, type, Line::LEFT));
	Homework1::addCollisionLine(Line(bottomRightCorner, topRightCorner, type, Line::RIGHT));

	return platform;
}