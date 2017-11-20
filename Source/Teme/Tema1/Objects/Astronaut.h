#pragma once

#include <include/glm.h>

namespace Astronaut {

	inline glm::vec3 GetTop(glm::vec3 center, float edge, float angle) {
		float medianLength = (((float)sqrt(3)) * edge) / 2;
		float totalOffset = medianLength * 2 / 3;
		float totalAngle = (M_PI / 2) + angle;
		float offsetX = totalOffset * cos(totalAngle);
		float offsetY = totalOffset * sin(totalAngle);
		return center + glm::vec3(offsetX, offsetY, 0);
	}

}