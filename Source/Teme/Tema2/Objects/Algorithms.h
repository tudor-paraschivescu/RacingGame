#pragma once

#include <stdio.h>

inline int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
	float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return 0; // Collinear
	bool denomPositive = denom > 0;

	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
		return 0; // No collision

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
		return 0; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
		return 0; // No collision
	
	// Collision detected
	t = t_numer / denom;
	if (i_x != NULL)
		*i_x = p0_x + (t * s10_x);
	if (i_y != NULL)
		*i_y = p0_y + (t * s10_y);

	return 1;
}

inline float DotProduct(glm::vec2 a, glm::vec2 b) {
	glm::vec2 product = a * b;
	float dotProduct = product[0] + product[1];
	return dotProduct;
}

inline bool detectCollisionBetweenLineAndCircle(float x1, float y1, float x2, float y2,
		CollisionCircle circle) {

	float radius = circle.getRadius();

	glm::vec2 d = glm::vec2(x2, y2) - glm::vec2(x1, y1);
	glm::vec2 f = glm::vec2(x1, y1) - glm::vec2(circle.getX(), circle.getY());

	// Equation parameters
	float a = DotProduct(d, d);
	float b = 2 * DotProduct(f, d);
	float c = DotProduct(f, f) - radius * radius;

	// The discriminant of the equation
	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) {
		// No intersection because the equation does not have a solution
		return false;
	}

	// The equation has one or two solutions
	discriminant = sqrt(discriminant);
	float sol1 = (-b - discriminant) / (2 * a);
	float sol2 = (-b + discriminant) / (2 * a);

	if ((sol1 >= 0 && sol1 <= 1) || (sol2 >= 0 && sol2 <= 1)) {
		// We have at least one valid solution
		return true;
	}

	// No collision detected for this line
	return false;
}