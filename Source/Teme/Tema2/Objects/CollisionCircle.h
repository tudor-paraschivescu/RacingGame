#pragma once

class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float radius);
	~CollisionCircle();
	float getX();
	float getY();
	float getRadius();

private:
	float _x;
	float _y;
	float _radius;
};

