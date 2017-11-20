#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <Core/Engine.h>
#include "Objects\Object.h"
#include "Objects\Line.h"
#include "Objects\Asteroid.h"
#include "Transformations\Math.h"
#include "Transformations\Transformation.h"
#include "Objects\Astronaut.h"

class Homework1 : public SimpleScene
{
public:
	Homework1();
	~Homework1();
	void Init() override;

	// Add a collision line for a platform
	static void addCollisionLine(Line line);

	static const int WINDOW_HEIGHT = 960;
	static const int WINDOW_WIDTH = 1720;

private:
	// Private Methods
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	bool detectPlatformCollision();
	int detectAsteroidCollision();
	void updateAstronautAfterCollision(int idxAsteroid);

	// Private constants for the meshes
	// ------------------------------ ASTRONAUT ------------------------------
	const std::string ASTRONAUT_NAME = "astronaut";
	const glm::vec3 ASTRONAUT_CENTER = glm::vec3(40, 40, 1);
	const float ASTRONAUT_EDGE_LENGTH = 25;
	const float ASTRONAUT_SPEED = 400;
	// -----------------------------------------------------------------------

	// ------------------------------ ASTEROIDS ------------------------------
	const std::string ASTEROID1_NAME = "asteroid1";
	const glm::vec3 ASTEROID1_CENTER = glm::vec3(150, 400, 1);
	const float ASTEROID1_RADIUS = 35;
	const int ASTEROID1_Y_TRANSLATION_FACTOR = 200;
	const int ASTEROID1_Y_LIMIT_UP = 300;
	const int ASTEROID1_Y_LIMIT_DOWN = 50;

	const std::string ASTEROID2_NAME = "asteroid2";
	const float ASTEROID2_OFFSET_X = 350;
	const float ASTEROID2_OFFSET_Y = 280;
	const glm::vec3 ASTEROID2_CENTER = glm::vec3(ASTEROID2_OFFSET_X, ASTEROID2_OFFSET_Y, 1);
	const float ASTEROID2_RADIUS = 90;
	const float ASTEROID2_SCALE_FACTOR_MIN = 0.5;
	const float ASTEROID2_SCALE_FACTOR_MAX = 1.5;
	const float ASTEROID2_SCALE_FACTOR = 0.40;

	const std::string ASTEROID3_NAME = "asteroid3";
	const glm::vec3 ASTEROID3_CENTER = glm::vec3(1400, 200, 1);
	const float ASTEROID3_RADIUS = 75;
	const float ASTEROID3_SCALE_FACTOR_MIN = 0.9;
	const float ASTEROID3_SCALE_FACTOR_MAX = 2.2;
	const float ASTEROID3_SCALE_FACTOR = 0.5;
	const float ASTEROID3_ROTATION_FACTOR = 1.6;
	const float ASTEROID3_OFFSET_X = 1400;
	const float ASTEROID3_OFFSET_Y = 200;

	const std::string ASTEROID4_NAME = "asteroid4";
	const glm::vec3 ASTEROID4_CENTER = glm::vec3(1450, 550, 1);
	const float ASTEROID4_RADIUS = 120;
	const float ASTEROID4_ROTATION_FACTOR = 2.0;
	const float ASTEROID4_OFFSET_X = 1450;
	const float ASTEROID4_OFFSET_Y = 550;
	// -----------------------------------------------------------------------

	// ------------------------------ PLATFORMS ------------------------------
	const float PLATFORM_THICKNESS = 20;

	const std::string WALL_UP_NAME = "wall_up";
	const glm::vec3 WALL_UP_BOTTOM_LEFT_CORNER = glm::vec3(0, WINDOW_HEIGHT - PLATFORM_THICKNESS, 1);
	const float WALL_UP_WIDTH = (float) WINDOW_WIDTH;

	const std::string WALL_DOWN_NAME = "wall_down";
	const glm::vec3 WALL_DOWN_BOTTOM_LEFT_CORNER = glm::vec3(0, 0, 1);
	const float WALL_DOWN_WIDTH = (float) WINDOW_WIDTH;

	const std::string WALL_LEFT_NAME = "wall_left";
	const glm::vec3 WALL_LEFT_BOTTOM_LEFT_CORNER = glm::vec3(0, 0, 1);
	const float WALL_LEFT_HEIGHT = (float) WINDOW_HEIGHT;

	const std::string WALL_RIGHT_NAME = "wall_right";
	const glm::vec3 WALL_RIGHT_BOTTOM_LEFT_CORNER = glm::vec3(WINDOW_WIDTH - PLATFORM_THICKNESS, 0, 1);
	const float WALL_RIGHT_HEIGHT = (float) WINDOW_HEIGHT;

	const std::string PLATFORM_STATIONARY1_NAME = "platform_stationary1";
	const glm::vec3 PLATFORM_STATIONARY1_BOTTOM_LEFT_CORNER = glm::vec3(100, 820, 1);
	const float PLATFORM_STATIONARY1_WIDTH = 150;

	const std::string PLATFORM_STATIONARY2_NAME = "platform_stationary2";
	const glm::vec3 PLATFORM_STATIONARY2_BOTTOM_LEFT_CORNER = glm::vec3(320, 720, 1);
	const float PLATFORM_STATIONARY2_WIDTH = 240;

	const std::string PLATFORM_STATIONARY3_NAME = "platform_stationary3";
	const glm::vec3 PLATFORM_STATIONARY3_BOTTOM_LEFT_CORNER = glm::vec3(900, 180, 1);
	const float PLATFORM_STATIONARY3_HEIGHT = 400;

	const std::string PLATFORM_BOUNCE1_NAME = "platform_bounce1";
	const glm::vec3 PLATFORM_BOUNCE1_BOTTOM_LEFT_CORNER = glm::vec3(700, 20, 1);
	const float PLATFORM_BOUNCE1_HEIGHT = 350;

	const std::string PLATFORM_BOUNCE2_NAME = "platform_bounce2";
	const glm::vec3 PLATFORM_BOUNCE2_BOTTOM_LEFT_CORNER = glm::vec3(700, 600, 1);
	const float PLATFORM_BOUNCE2_HEIGHT = 350;

	const std::string PLATFORM_BOUNCE3_NAME = "platform_bounce3";
	const glm::vec3 PLATFORM_BOUNCE3_BOTTOM_LEFT_CORNER = glm::vec3(1000, 650, 1);
	const float PLATFORM_BOUNCE3_HEIGHT = 300;

	const std::string PLATFORM_FINISH_NAME = "platform_finish";
	const glm::vec3 PLATFORM_FINISH_BOTTOM_LEFT_CORNER = glm::vec3(WINDOW_WIDTH - 2 * PLATFORM_THICKNESS, 200, 1);
	const float PLATFORM_FINISH_HEIGHT = 300;
	// -----------------------------------------------------------------------
	
	// Private variables
	glm::mat3 modelMatrix;			// the matrix used for transformations in the Update method
	Asteroid* collisionAsteroids[4];// stores collision asteroids
	float tx1;						// translation value for Ox (asteroid1)
	float ty1;						// translation value for Oy (asteroid1)
	bool up1;						// condition variable for going up or down (asteroid1)
	float scaleFactor2;				// scale factor value (asteroid2)
	bool grow2;						// condition variable for growing or shrinking (asteroid2)
	float rotationAngle3;			// angle for the rotation (asteroid3)
	float scaleFactor3;				// scale factor value (asteroid3)
	bool grow3;						// condition variable for growing or shrinking (asteroid3)
	float rotationAngle4;			// angle for the rotation (asteroid4)

	glm::vec3 centerOfAstronaut;			// coordinates for the center of the Astronaut
	float txA;								// translation for the Astronaut on Ox axis
	float tyA;								// translation for the Astronaut on Oy axis
	float rotationAngleOfAstronaut;			// angle of the Astronaut movement (in [-90, 270] deg interval)
	bool canAstronautChangeDirection;		// condition variable that is activated when the Astronaut is staying
	bool mouseClick;						// condition variable to determine if a mouse click ocurred
	bool onPlatform;						// condition variable to determine if the astronaut is on a platform

	// Private stats that will show up after game ends
	int _acceptedClicks = 0;
	int _rejectedClicks = 0;
	int _collisions = 0;
	int _asteroidHits = 0;
	// -----------------------------------------------------------------------

	// ------------------ Animations for all moving objects ------------------

	void animateAsteroid1(float deltaTimeSeconds) {
		if (ty1 > ASTEROID1_Y_LIMIT_UP) {
			up1 = false;			// Asteroid 1 must change its direction and go down
		}
		else if (ty1 < ASTEROID1_Y_LIMIT_DOWN) {
			up1 = true;				// Asteroid 1 must change its direction and go up
		}

		float frameTranslation = deltaTimeSeconds * ASTEROID1_Y_TRANSLATION_FACTOR;
		if (!up1) {
			frameTranslation = -frameTranslation;
		}

		ty1 += frameTranslation;	// Update translation factor for Oy

		// Calculate the transformation matrix
		modelMatrix = Transformation::Translate(tx1, ty1);

		// Update center from vector of collision asteroids
		if (collisionAsteroids[0] != NULL) {
			collisionAsteroids[0]->setCenter(ASTEROID1_CENTER[0] + tx1, ASTEROID1_CENTER[1] + ty1);
		}
	}

	void animateAsteroid2(float deltaTimeSeconds) {
		if (scaleFactor2 > ASTEROID2_SCALE_FACTOR_MAX) {
			grow2 = false;			// Asteroid 2 must shrink
		}
		else if (scaleFactor2 < ASTEROID2_SCALE_FACTOR_MIN) {
			grow2 = true;			// Asteroid 2 must grow
		}

		float frameScaleFactor = deltaTimeSeconds * ASTEROID2_SCALE_FACTOR;
		if (!grow2) {
			frameScaleFactor = -frameScaleFactor;
		}

		scaleFactor2 += frameScaleFactor;	// Update scale factor

		// Calculate the transformation matrix
		modelMatrix = Transformation::Translate(ASTEROID2_OFFSET_X, ASTEROID2_OFFSET_Y);
		modelMatrix *= Transformation::Scale(scaleFactor2, scaleFactor2);
		modelMatrix *= Transformation::Translate(-ASTEROID2_OFFSET_X, -ASTEROID2_OFFSET_Y);

		// Update radius from vector of collision asteroids
		if (collisionAsteroids[1] != NULL) {
			collisionAsteroids[1]->setRadius(scaleFactor2 * ASTEROID2_RADIUS);
		}
	}

	void animateAsteroid3(float deltaTimeSeconds) {
		// Update the angle
		rotationAngle3 += deltaTimeSeconds * ASTEROID3_ROTATION_FACTOR;

		if (scaleFactor3 > ASTEROID3_SCALE_FACTOR_MAX) {
			grow3 = false;			// Asteroid 3 must shrink
		}
		else if (scaleFactor3 < ASTEROID3_SCALE_FACTOR_MIN) {
			grow3 = true;			// Asteroid 3 must grow
		}

		float frameScaleFactor = deltaTimeSeconds * ASTEROID3_SCALE_FACTOR;
		if (!grow3) {
			frameScaleFactor = -frameScaleFactor;
		}
		
		scaleFactor3 += frameScaleFactor;	// Update scale factor

		// Calculate the transformation matrix
		modelMatrix = Transformation::Translate(ASTEROID3_OFFSET_X, ASTEROID3_OFFSET_Y);
		modelMatrix *= Transformation::Rotate(rotationAngle3);
		modelMatrix *= Transformation::Scale(scaleFactor3, scaleFactor3);
		modelMatrix *= Transformation::Translate(-ASTEROID3_OFFSET_X, -ASTEROID3_OFFSET_Y);

		// Update radius from vector of collision asteroids
		if (collisionAsteroids[2] != NULL) {
			collisionAsteroids[2]->setRadius(scaleFactor3 * ASTEROID3_RADIUS);
		}
	}

	void animateAsteroid4(float deltaTimeSeconds) {
		// Update the angle
		rotationAngle4 += deltaTimeSeconds * ASTEROID4_ROTATION_FACTOR;

		// Calculate the transformation matrix
		modelMatrix = Transformation::Translate(ASTEROID4_OFFSET_X, ASTEROID4_OFFSET_Y);
		modelMatrix *= Transformation::Rotate(rotationAngle4);
		modelMatrix *= Transformation::Translate(-ASTEROID4_OFFSET_X, -ASTEROID4_OFFSET_Y);
	}

	void animateAstronaut(float deltaTimeSeconds) {

		if (canAstronautChangeDirection) {
			if (mouseClick) {
				// The astronaut cannot change its direction after the mouse click
				canAstronautChangeDirection = false;
			}
		}
		else {
			bool copyOfOnPlatform = onPlatform;
			int idxAsteroidCollision = detectAsteroidCollision();
			if ((detectPlatformCollision() && !copyOfOnPlatform) || idxAsteroidCollision != -1) {

				if (idxAsteroidCollision != -1) {
					std::cout << "[-- ASTEROID HIT --] @ " << std::setw(7) << centerOfAstronaut[0]
						<< " " << std::setw(7) << centerOfAstronaut[1] << std::endl;
					_asteroidHits++;
				}
				else {
					std::cout << "[PLATFORM COLLISION] @ " << std::setw(7) << centerOfAstronaut[0]
						<< " " << std::setw(7) << centerOfAstronaut[1] << std::endl;
					_collisions++;
				}

				updateAstronautAfterCollision(idxAsteroidCollision);
			}
			else {
				float updatedSpeed = deltaTimeSeconds * ASTRONAUT_SPEED;

				// Update the translation factors for both axis
				txA += updatedSpeed * cos(M_PI / 2 + rotationAngleOfAstronaut);
				tyA += updatedSpeed * sin(M_PI / 2 + rotationAngleOfAstronaut);
			}
		}

		// Move the astronaut
		modelMatrix = Transformation::Translate(ASTRONAUT_CENTER[0] + txA, ASTRONAUT_CENTER[1] + tyA);
		modelMatrix *= Transformation::Rotate(rotationAngleOfAstronaut);
		modelMatrix *= Transformation::Translate(-ASTRONAUT_CENTER[0], -ASTRONAUT_CENTER[1]);

		// Update the center of the astronaut
		centerOfAstronaut[0] = ASTRONAUT_CENTER[0] + txA;
		centerOfAstronaut[1] = ASTRONAUT_CENTER[1] + tyA;
	}

};
