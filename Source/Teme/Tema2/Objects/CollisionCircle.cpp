#include "CollisionCircle.h"

CollisionCircle::CollisionCircle()
{
}

CollisionCircle::CollisionCircle(float x, float y, float radius)
{
	_x = x;
	_y = y;
	_radius = radius;
}

CollisionCircle::~CollisionCircle()
{
}

float CollisionCircle::getX()
{
	return _x;
}

float CollisionCircle::getY()
{
	return _y;
}

float CollisionCircle::getRadius()
{
	return _radius;
}
