#pragma once

#include "../../../include/glm.h"
#include "Object.h"

class Line
{

public:
	// Enumeration of the possible collision lines types
	enum CollisionLineType { BOTTOM, TOP, LEFT, RIGHT };

	Line(glm::vec3 firstPoint, glm::vec3 secondPoint,
			Object::PlatformType platformType, CollisionLineType lineType);
	~Line();
	glm::vec3 getFirstPoint();
	glm::vec3 getSecondPoint();
	Object::PlatformType getPlatformType();
	CollisionLineType getCollisionLineType();

private:
	glm::vec3 _firstPoint;
	glm::vec3 _secondPoint;
	Object::PlatformType _platformType;
	CollisionLineType _lineType;
};

