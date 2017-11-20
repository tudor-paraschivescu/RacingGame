#include "Homework1.h"

using namespace std;

vector<Line> collisionLines;			// stores collision lines for all the platforms
int indexOfCollisionLine = -1;			// stores the index of the line where the last collision ocurred

Homework1::Homework1()
{
}

Homework1::~Homework1()
{
}

void Homework1::Init()
{
	// Set the camera
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// Variable initialization
	tx1 = 0;
	ty1 = 0;
	up1 = true;
	scaleFactor2 = 1;
	grow2 = true;
	rotationAngle3 = 0;
	scaleFactor3 = 1;
	grow3 = true;
	rotationAngle4 = 0;
	centerOfAstronaut = ASTRONAUT_CENTER;
	rotationAngleOfAstronaut = 0;
	txA = 0;
	tyA = 0;
	canAstronautChangeDirection = true;
	mouseClick = false;
	onPlatform = false;

	// Set precision limit for output
	cout << fixed << setprecision(2);

	// Create and add the meshes to the list
	// -----------------------------------------------------------------------
	// The astronaut
	AddMeshToList(Object::CreateAstronaut(ASTRONAUT_NAME, ASTRONAUT_CENTER, ASTRONAUT_EDGE_LENGTH));

	// The walls
	AddMeshToList(Object::CreatePlatform(WALL_UP_NAME, WALL_UP_BOTTOM_LEFT_CORNER,
		PLATFORM_THICKNESS, WALL_UP_WIDTH, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(WALL_DOWN_NAME, WALL_DOWN_BOTTOM_LEFT_CORNER,
		PLATFORM_THICKNESS, WALL_DOWN_WIDTH, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(WALL_LEFT_NAME, WALL_LEFT_BOTTOM_LEFT_CORNER,
		WALL_LEFT_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(WALL_RIGHT_NAME, WALL_RIGHT_BOTTOM_LEFT_CORNER,
		WALL_RIGHT_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::STATIONARY));

	// The stationary, bounce and finish platforms
	AddMeshToList(Object::CreatePlatform(PLATFORM_STATIONARY1_NAME, PLATFORM_STATIONARY1_BOTTOM_LEFT_CORNER,
		PLATFORM_THICKNESS, PLATFORM_STATIONARY1_WIDTH, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(PLATFORM_STATIONARY2_NAME, PLATFORM_STATIONARY2_BOTTOM_LEFT_CORNER,
		PLATFORM_THICKNESS, PLATFORM_STATIONARY2_WIDTH, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(PLATFORM_STATIONARY3_NAME, PLATFORM_STATIONARY3_BOTTOM_LEFT_CORNER,
		PLATFORM_STATIONARY3_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::STATIONARY));
	AddMeshToList(Object::CreatePlatform(PLATFORM_BOUNCE1_NAME, PLATFORM_BOUNCE1_BOTTOM_LEFT_CORNER,
		PLATFORM_BOUNCE1_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::BOUNCE));
	AddMeshToList(Object::CreatePlatform(PLATFORM_BOUNCE2_NAME, PLATFORM_BOUNCE2_BOTTOM_LEFT_CORNER,
		PLATFORM_BOUNCE2_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::BOUNCE));
	AddMeshToList(Object::CreatePlatform(PLATFORM_BOUNCE3_NAME, PLATFORM_BOUNCE3_BOTTOM_LEFT_CORNER,
		PLATFORM_BOUNCE3_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::BOUNCE));
	AddMeshToList(Object::CreatePlatform(PLATFORM_FINISH_NAME, PLATFORM_FINISH_BOTTOM_LEFT_CORNER,
		PLATFORM_FINISH_HEIGHT, PLATFORM_THICKNESS, Object::PlatformType::FINISH));

	// The asteroids
	AddMeshToList(Object::CreateAsteroid(ASTEROID1_NAME, ASTEROID1_CENTER, ASTEROID1_RADIUS));
	AddMeshToList(Object::CreateAsteroid(ASTEROID2_NAME, ASTEROID2_CENTER, ASTEROID2_RADIUS));
	AddMeshToList(Object::CreateAsteroid(ASTEROID3_NAME, ASTEROID3_CENTER, ASTEROID3_RADIUS));
	AddMeshToList(Object::CreateAsteroid(ASTEROID4_NAME, ASTEROID4_CENTER, ASTEROID4_RADIUS));
	// -----------------------------------------------------------------------

	// Add the collision asteroids in the array where they are kept track of
	collisionAsteroids[0] = new Asteroid(ASTEROID1_CENTER, ASTEROID1_RADIUS);
	collisionAsteroids[1] = new Asteroid(ASTEROID2_CENTER, ASTEROID2_RADIUS);
	collisionAsteroids[2] = new Asteroid(ASTEROID3_CENTER, ASTEROID3_RADIUS);
	collisionAsteroids[3] = new Asteroid(ASTEROID4_CENTER, ASTEROID4_RADIUS);
}

void Homework1::addCollisionLine(Line line)
{
	collisionLines.push_back(line);
}

void Homework1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// Render walls
	RenderMesh2D(meshes[WALL_UP_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[WALL_DOWN_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[WALL_LEFT_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[WALL_RIGHT_NAME], shaders["VertexColor"], glm::mat3(1));

	// Render platforms
	RenderMesh2D(meshes[PLATFORM_STATIONARY1_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_STATIONARY2_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_STATIONARY3_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_BOUNCE1_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_BOUNCE2_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_BOUNCE3_NAME], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes[PLATFORM_FINISH_NAME], shaders["VertexColor"], glm::mat3(1));
}

void Homework1::Update(float deltaTimeSeconds)
{
	// Animate, update and render asteroid 1
	animateAsteroid1(deltaTimeSeconds);
	if (collisionAsteroids[0] != NULL) {
		RenderMesh2D(meshes[ASTEROID1_NAME], shaders["VertexColor"], modelMatrix);
	}

	// Animate, update and render asteroid 2
	animateAsteroid2(deltaTimeSeconds);
	if (collisionAsteroids[1] != NULL) {
		RenderMesh2D(meshes[ASTEROID2_NAME], shaders["VertexColor"], modelMatrix);
	}

	// Animate, update and render asteroid 3
	animateAsteroid3(deltaTimeSeconds);
	if (collisionAsteroids[2] != NULL) {
		RenderMesh2D(meshes[ASTEROID3_NAME], shaders["VertexColor"], modelMatrix);
	}
	
	// Animate, update and render asteroid 4
	animateAsteroid4(deltaTimeSeconds);
	if (collisionAsteroids[3] != NULL) {
		RenderMesh2D(meshes[ASTEROID4_NAME], shaders["VertexColor"], modelMatrix);
	}

	// Animate and render astronaut
	animateAstronaut(deltaTimeSeconds);
	RenderMesh2D(meshes[ASTRONAUT_NAME], shaders["VertexColor"], modelMatrix);
}

void Homework1::FrameEnd()
{
}

void Homework1::OnInputUpdate(float deltaTime, int mods)
{
}

void Homework1::OnKeyPress(int key, int mods)
{
}

void Homework1::OnKeyRelease(int key, int mods)
{
}

void Homework1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Homework1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Last click position in coordinates starting from bottom-left corner
	glm::vec3 lastClickPosition = { mouseX, WINDOW_HEIGHT - mouseY, 1 };

	if (canAstronautChangeDirection) {
		// Calculate the position of the astronaut top
		glm::vec3 top = Astronaut::GetTop(centerOfAstronaut, ASTRONAUT_EDGE_LENGTH, rotationAngleOfAstronaut);

		// Calculate the angle between the old and the new direction of movement
		float offsetAngle = Math::AngleBetween3Points(centerOfAstronaut, top, lastClickPosition);

		if (onPlatform && cos(offsetAngle) <= 0) {
			// Ignore mouse click event to prevent passing through a platform
			std::cout << "[- REJECTED CLICK -] @ " << setw(7) << lastClickPosition[0]
				<< " " << setw(7) << lastClickPosition[1] << std::endl;
			_rejectedClicks++;
			return;
		}

		// Calculate the angle of the new direction
		rotationAngleOfAstronaut += offsetAngle;

		// Mouse click event happened
		mouseClick = true;

		std::cout << "[- ACCEPTED CLICK -] @ " << setw(7) << lastClickPosition[0]
			<< " " << setw(7) << lastClickPosition[1] << std::endl;
		_acceptedClicks++;
	}
	else {
		// Ignore mouse click event to prevent changing the direction during movement
		std::cout << "[- REJECTED CLICK -] @ " << setw(7) << lastClickPosition[0]
			<< " " << setw(7) << lastClickPosition[1] << std::endl;
		_rejectedClicks++;
	}
}

void Homework1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Mouse click event stopped
	mouseClick = false;
}

void Homework1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework1::OnWindowResize(int width, int height)
{
}

bool Homework1::detectPlatformCollision() {

	bool isCollision = false;

	for (int i = 0; i < collisionLines.size(); i++) {

		// Store the properties of the ray (line segment) and the circle around the astronaut
		glm::vec3 firstLinePoint = collisionLines[i].getFirstPoint();
		glm::vec3 secondLinePoint = collisionLines[i].getSecondPoint();
		glm::vec3 centerOfCircle = centerOfAstronaut;
		float radius = (((float)sqrt(3)) * ASTRONAUT_EDGE_LENGTH) / 3.0;

		glm::vec3 d = secondLinePoint - firstLinePoint; // Direction vector of ray (start to end)
		glm::vec3 f = firstLinePoint - centerOfCircle;	// Vector from circle center to ray start

		/*
		 *	The intersection is found by Plugging: P = E + t * d
		 *	This is a parametric equation: Px = Ex + tdx, Py = Ey + tdy
		 *	The form wil be: (x - cx)^2 + (y - cy)^2 = r^2, (cx, cy) = center of circle
		 *	--------------------- Lots of calculus later ----------------------
		 *	Final form of the equation: t^2 * (d .* d) + 2t * ( f .* d ) + ( f .* f - r^2 ) = 0
		 *	where .* is DOT PRODUCT of two vectors
		 *	NEXT: Solving the equation...
		 */

		// Equation parameters
		float a = Math::DotProduct(d, d);
		float b = 2 * Math::DotProduct(f, d);
		float c = Math::DotProduct(f, f) - radius * radius;

		// The discriminant of the equation
		float discriminant = b * b - 4 * a * c;

		if (discriminant < 0) {
			// No intersection because the equation does not have a solution
			continue;
		}

		// The equation has one or two solutions
		discriminant = sqrt(discriminant);
		float sol1 = (-b - discriminant) / (2 * a);
		float sol2 = (-b + discriminant) / (2 * a);

		if ((sol1 >= 0 && sol1 <= 1) || (sol2 >= 0 && sol2 <= 1)) {
			// We have at least one valid solution
			isCollision = true;
			indexOfCollisionLine = i;
			onPlatform = true;
			break;
		}

		// No collision detected for this line
	}

	// The astronaut left the platform
	if (!isCollision && onPlatform) {
		onPlatform = false;
	}

	return isCollision;
}

int Homework1::detectAsteroidCollision()
{
	for (int i = 0; i < 4; i++)
	{
		if (collisionAsteroids[i] != NULL)
		{
			float dx = collisionAsteroids[i]->getCenter()[0] - centerOfAstronaut[0];
			float dy = collisionAsteroids[i]->getCenter()[1] - centerOfAstronaut[1];
			float totalR = ASTRONAUT_EDGE_LENGTH + collisionAsteroids[i]->getRadius();

			if (dx * dx + dy * dy <= totalR * totalR)
			{
				// Collision with asteroid at index i
				return i;
			}
		}
	}

	// No asteroid collision
	return -1;
}

void Homework1::updateAstronautAfterCollision(int idxAsteroid)
{
	// Update the astronaut after an asteroid collision
	if (idxAsteroid != -1) {
		collisionAsteroids[idxAsteroid] = NULL;
		rotationAngleOfAstronaut = -rotationAngleOfAstronaut;
		return;
	}

	Line collisionLine = collisionLines[indexOfCollisionLine];
	Line::CollisionLineType lineType = collisionLine.getCollisionLineType();

	// Update the astronaut after a platform collision
	switch (collisionLine.getPlatformType()) {

	case (Object::PlatformType::STATIONARY):
	{
		// Astronaut is on a stationary platform and can change direction
		canAstronautChangeDirection = true;

		// Calculate the distance from the center of the triangle to the platform
		float aLine = collisionLine.getSecondPoint()[1] - collisionLine.getFirstPoint()[1];
		float bLine = collisionLine.getSecondPoint()[0] - collisionLine.getFirstPoint()[0];
		float cLine = collisionLine.getFirstPoint()[0] * collisionLine.getSecondPoint()[1] -
			collisionLine.getSecondPoint()[0] * collisionLine.getFirstPoint()[1];
		float centerLine = aLine * centerOfAstronaut[0] + bLine * centerOfAstronaut[1] + cLine;
		float distanceToPlatform = abs(centerLine / sqrt(aLine * aLine + bLine * bLine));

		// Calculate the distance to the base of the triangle
		float distanceToBase = (((float)sqrt(3)) * ASTRONAUT_EDGE_LENGTH) / 6;

		// Calculate the offset to the platform
		float offset = distanceToPlatform - distanceToBase;

		/* I need this because some weird shit is going on with
		* the offset for left and right platform */
		float offsetRight = (centerOfAstronaut[0] - collisionLine.getFirstPoint()[0]) - distanceToBase;
		float offsetLeft = -(centerOfAstronaut[0] - collisionLine.getFirstPoint()[0]) - distanceToBase;

		switch (lineType)
		{
		case Line::CollisionLineType::BOTTOM:
			rotationAngleOfAstronaut = M_PI;
			tyA += offset;
			break;

		case Line::CollisionLineType::TOP:
			rotationAngleOfAstronaut = 0;
			tyA -= offset;
			break;

		case Line::CollisionLineType::LEFT:
			rotationAngleOfAstronaut = M_PI / 2;
			txA += offsetLeft;
			break;

		case Line::CollisionLineType::RIGHT:
			rotationAngleOfAstronaut = 3 * M_PI / 2;
			txA -= offsetRight;
			break;

		default:
			break;
		}

		break;
	}

	case (Object::PlatformType::BOUNCE):
	{
		switch (lineType)
		{
		case Line::CollisionLineType::BOTTOM:
			rotationAngleOfAstronaut = rotationAngleOfAstronaut - M_PI;
			break;

		case Line::CollisionLineType::TOP:
			rotationAngleOfAstronaut = rotationAngleOfAstronaut - M_PI;
			break;

		case Line::CollisionLineType::LEFT:
			rotationAngleOfAstronaut = -rotationAngleOfAstronaut;
			break;

		case Line::CollisionLineType::RIGHT:
			rotationAngleOfAstronaut = -rotationAngleOfAstronaut;
			break;

		default:
			break;
		}

	break;
	}
	
	case (Object::PlatformType::FINISH):
	{
		// Close the window
		Engine::GetWindow()->Close();
		cout << "[-- WINDOW CLOSED -]\n-----------------------------------------" << endl;

		// Display WINNING MESSAGE
		cout << "\n*** OMG! YOU WON! YOU ARE ONE OF THE GREATEST ASTRONAUTS I HAVE EVER SEEN! ***\n" << endl;

		// Display stats
		cout << "Some stats of your intergalactic journey: " << endl;
		cout << "[ASTEROID HITS: " << _asteroidHits << "] -> ";
		for (int i = 0; i < _asteroidHits; i++) {
			cout << "*";
		}
		cout << endl;
		cout << "[PLATFORM COLLISIONS: " << _collisions << "] -> ";
		for (int i = 0; i < _collisions; i++) {
			cout << "*";
		}
		cout << endl;
		cout << "[ACCEPTED CLICKS: " << _acceptedClicks << "] -> ";
		for (int i = 0; i < _acceptedClicks; i++) {
			cout << "*";
		}
		cout << endl;
		cout << "[REJECTED CLICKS: " << _rejectedClicks << "] -> ";
		for (int i = 0; i < _rejectedClicks; i++) {
			cout << "*";
		}
		cout << endl << endl;
	}

	default:
		break;
	}

}
