#pragma once

#include <include/glm.h>

namespace Color {

	// Normalized RGB Colors for the astronaut
	const glm::vec3 ASTRONAUT_BASE = { 0.10, 0.10, 0.85 };
	const glm::vec3 ASTRONAUT_TOP = { 0.85, 0.10, 0.10 };

	// Normalized RGB Color for a stationary platform
	const glm::vec3 PLATFORM_STATIONARY = { 0.18, 0.18, 0.18 };

	// Normalized RGB Color for a bounce platform
	const glm::vec3 PLATFORM_BOUNCE = { 0.75, 0.75, 0.15 };

	// Normalized RGB Color for the finish platform
	const glm::vec3 PLATFORM_FINISH = { 0.9, 0.1, 0.1 };

	// Normalized RGB Colors for an asteroid
	const glm::vec3 ASTEROID_CENTER = { 0.15, 0.50, 0.15 };
	const glm::vec3 ASTEROID_ARCH = { 0.15, 0.75, 0.15 };

}