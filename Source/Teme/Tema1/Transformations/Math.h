#pragma once

#include <include/glm.h>

namespace Math {

	inline float AngleBetween3Points(glm::vec3 center, glm::vec3 top, glm::vec3 click)
	{
		float angle1 = atan2(center[1] - top[1], center[0] - top[0]);
		float angle2 = atan2(center[1] - click[1], center[0] - click[0]);
		float angleBetweenLines = angle2 - angle1;
		return angleBetweenLines;
	}

	inline float DotProduct(glm::vec3 a, glm::vec3 b) {
		glm::vec3 product = a * b;
		float dotProduct = product[0] + product[1] + product[2];
		return dotProduct;
	}

}