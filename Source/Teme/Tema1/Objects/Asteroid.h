#pragma once

#include "../../../include/glm.h"

class Asteroid
{
public:
	Asteroid();
	Asteroid(glm::vec3 center, float radius);
	~Asteroid();
	glm::vec3 getCenter();
	void setCenter(float x, float y);
	float getRadius();
	void setRadius(float newRadius);

private:
	glm::vec3 _center;
	float _radius;
};

