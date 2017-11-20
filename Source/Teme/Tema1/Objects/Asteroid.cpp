#include "Asteroid.h"

Asteroid::Asteroid()
{
}

Asteroid::Asteroid(glm::vec3 center, float radius)
{
	_center = center;
	_radius = radius;
}

Asteroid::~Asteroid()
{
}

glm::vec3 Asteroid::getCenter()
{
	return _center;
}

void Asteroid::setCenter(float x, float y)
{
	_center = { x, y, 1 };
}

float Asteroid::getRadius()
{
	return _radius;
}

void Asteroid::setRadius(float newRadius)
{
	_radius = newRadius;
}
