#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Teme\Tema1\Objects\Color.h>

namespace Object
{
	// The number of steps made in the discrete calculus for the Asteroid
	const int DISCRETE_STEPS = 16;

	// Enumeration of the possible platform types
	enum PlatformType { STATIONARY, BOUNCE, FINISH };

	// Create astronaut in given point with given edge and color
	Mesh* CreateAstronaut(	std::string name,
							glm::vec3 center,
							float edge);

	// Create asteroid with the given center, radius and color
	Mesh* CreateAsteroid(	std::string name,
							glm::vec3 center,
							float radius);

	// Create platform with the given bottom left corner, height, width and color
	Mesh* CreatePlatform(	std::string name,
							glm::vec3 bottomLeftCorner,
							float height,
							float width,
							PlatformType type);
}

