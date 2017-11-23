#include "Homework2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Data structures that keep certain meshes
vector<Mesh *> roadPartsMeshes;
vector<Mesh *> bordersMeshes;
vector<Mesh *> obstacles;

// Data structures that keep certain coordinates
struct Line {
	float x1;
	float y1;
	float x2;
	float y2;
};
vector<Line> collisionBorderLines;
Line finishLine;
vector<float> obsCoords;
vector<CollisionCircle> collisionCircles;

// Counter for the occurred collisions
int occurredCollisions = 0;

// Variable that represents the current hour (in 24h format)
float hourOfTheDay = 10.0;

// Variables used in solid and wireframe draw
GLenum cullFace;
GLenum polygonMode;

// Variables used in movement of the car
float tireRotation = 0;
float angleOfMovement = 0;
float currentGear = 0;

// Condition variable for movement of the car
bool rotateLeft = false;
bool rotateRight = false;

// Condition variable for camera type
bool isCameraFPS = false;

// Highscore data
float bestTime;
int bestLives;

Homework2::Homework2()
{
}

Homework2::~Homework2()
{
}

void Homework2::Init()
{
	cullFace = GL_BACK;
	polygonMode = GL_FILL;

	// Open track config file
	ifstream trackConfigFile;
	trackConfigFile.open(PATH_TO_CONFIG_FILE, ios::in);
	if (!trackConfigFile.is_open()) {
		cout << "ERROR at opening track configuration file" << endl;
	}

	// Data structures used for storing the road
	enum RoadPartType { LEFT, STRAIGHT, RIGHT };
	struct _MyRoadPart {
		RoadPartType type;
		int length;
	};
	vector<_MyRoadPart> roadParts;

	// Read and store data from track config file
	// It is assumed that the input file has a correct syntax
	string line;
	while (getline(trackConfigFile, line)) {
		// Tokenize the line
		char *startOfLine = &line[0];
		vector<string> lineTokens;
		do {
			const char *begin = startOfLine;
			while (*startOfLine != ' ' && *startOfLine)
				startOfLine++;
			lineTokens.push_back(string(begin, startOfLine - begin));
		} while (0 != *startOfLine++);

		// Create the new road part with the specified data
		_MyRoadPart newRoadPart;
		if (lineTokens[0].compare(STRAIGHT_LINE) == 0) {
			newRoadPart.type = RoadPartType::STRAIGHT;
		} else if (lineTokens[0].compare(LEFT_LINE) == 0) {
			newRoadPart.type = RoadPartType::LEFT;
		} else {
			newRoadPart.type = RoadPartType::RIGHT;
		}
		newRoadPart.length = stoi(lineTokens[1], NULL, 10);

		// Add the new road part in the vector
		roadParts.push_back(newRoadPart);
	}

	// Close track config file
	trackConfigFile.close();

	// Open track highscore file
	ifstream highscoreFile;
	highscoreFile.open(PATH_TO_HIGHSCORE_FILE, ios::in);
	if (!highscoreFile.is_open()) {
		cout << "ERROR at opening highscore file" << endl;
	}

	// Read and store best high score from file
	while (getline(highscoreFile, line)) {
		// Tokenize the line
		char *startOfLine = &line[0];
		vector<string> lineTokens;
		do {
			const char *begin = startOfLine;
			while (*startOfLine != ' ' && *startOfLine)
				startOfLine++;
			lineTokens.push_back(string(begin, startOfLine - begin));
		} while (0 != *startOfLine++);

		bestTime = stof(lineTokens[0], NULL);
		bestLives = stoi(lineTokens[1], NULL, 10);
	}

	// Close highscore file
	highscoreFile.close();
	
	// Variables initialization
	float currX = ROAD_START_X;
	float currZ = ROAD_START_Z;
	int indexRoadPart = 0;

	// Create a vector that will contain all road parts meshes
	for (int i = 0; i < roadParts.size(); i++)
	{
		_MyRoadPart thisRoadPart = roadParts[i];
		bool isStraightLine = false;

		// Calculate X and Z offsets
		float offsetX = 0;
		float offsetZ = 0;
		if (thisRoadPart.type == RoadPartType::STRAIGHT) {
			offsetZ = -Road::ROADPART_LENGTH;
			isStraightLine = true;
		}
		else if (thisRoadPart.type == RoadPartType::LEFT) {
			offsetX = -Road::ROADPART_LENGTH;
		}
		else {
			offsetX = Road::ROADPART_LENGTH;
		}

		// Determine which borders are needed
		vector<Road::BorderType> types;
		vector<Road::BorderType> lastPartTypes;
		if (isStraightLine) {
			types.push_back(Road::BorderType::RIGHT);
			types.push_back(Road::BorderType::LEFT);
			
			if (i + 1 < roadParts.size()) {
				lastPartTypes.push_back(Road::BorderType::TOP);
				if (roadParts[i + 1].type == RoadPartType::LEFT) {
					lastPartTypes.push_back(Road::BorderType::RIGHT);
				}
				else {
					lastPartTypes.push_back(Road::BorderType::LEFT);
				}
			}
			else {
				lastPartTypes.push_back(Road::BorderType::RIGHT);
				lastPartTypes.push_back(Road::BorderType::LEFT);
			}
		}
		else {
			types.push_back(Road::BorderType::TOP);
			types.push_back(Road::BorderType::BOTTOM);

			lastPartTypes.push_back(Road::BorderType::BOTTOM);
			if (i + 1 == roadParts.size()) {
				lastPartTypes.push_back(Road::BorderType::TOP);
			} else if (roadParts[i].type == RoadPartType::LEFT) {
				lastPartTypes.push_back(Road::BorderType::LEFT);
			}
			else {
				lastPartTypes.push_back(Road::BorderType::RIGHT);
			}
		}

		while (thisRoadPart.length--)
		{
			// Update paramaters for each mesh
			string name = ROADPART_PREFIX + to_string(indexRoadPart);
			currX += offsetX;
			currZ += offsetZ;

			// Create each mesh on that road part
			Mesh *newMesh = CreateMesh(name, glm::vec3(currX, 0, currZ), (Road::BorderType)NULL);
			roadPartsMeshes.push_back(newMesh);

			// Create the meshes for the borders
			name = BORDER_PREFIX + to_string(indexRoadPart);
			Road::BorderType type1 = (thisRoadPart.length != 0) ? types[0] : lastPartTypes[0];
			Road::BorderType type2 = (thisRoadPart.length != 0) ? types[1] : lastPartTypes[1];
			
			Mesh *newBorder1 = CreateMesh(name + "_1", glm::vec3(currX, 0, currZ), type1);
			bordersMeshes.push_back(newBorder1);
			Mesh *newBorder2 = CreateMesh(name + "_2", glm::vec3(currX, 0, currZ), type2);
			bordersMeshes.push_back(newBorder2);

			indexRoadPart++;
		}
	}

	// Set the finish line
	{
		finishLine.x1 = currX;
		finishLine.y1 = currZ;
		finishLine.x2 = currX + 1;
		finishLine.y2 = currZ - Road::ROADPART_LENGTH;
	}
	
	// Create the meshes for earth and sky
	Mesh *earth = CreateMesh(EARTH_PREFIX, (glm::vec3)NULL, (Road::BorderType)NULL);
	Mesh* sky = CreateMesh(SKY_PREFIX, (glm::vec3)NULL, (Road::BorderType)NULL);
	
	// Variable used in generating obstacles for teapot-sphere alternation
	bool isTeapot = true;

	// Create the obstacles
	for (int i = 0; i < COUNT_OBSTACLES; i++) {
		Mesh* obstacle = new Mesh(OBSTACLE_PREFIX + to_string(i + 1));
		
		if (isTeapot) {
			obstacle->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		}
		else {
			obstacle->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		}

		meshes[obstacle->GetMeshID()] = obstacle;
		obstacles.push_back(obstacle);
		obsCoords.push_back(OBSTACLE_COORDS[2 * i]);
		obsCoords.push_back(OBSTACLE_COORDS[2 * i + 1]);
		CollisionCircle c = CollisionCircle(OBSTACLE_COORDS[2 * i], OBSTACLE_COORDS[2 * i + 1], OBSTACLE_RADIUS);
		collisionCircles.push_back(c);
		isTeapot = !isTeapot;
	}

	// Create the tires
	for (int i = 0; i < COUNT_TIRES; i++) {
		Mesh* tire = new Mesh(TIRE_PREFIX + to_string(i));
		tire->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "tire.obj");
		meshes[tire->GetMeshID()] = tire;
	}

	// Create the car
	Mesh* car = CreateMesh(CAR_PREFIX, (glm::vec3)NULL, (Road::BorderType)NULL);

	// Create a shader program for drawing face polygon with the color of the normal
	Shader *shader = new Shader(SHADER_NAME);
	shader->AddShader("Source/Teme/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	cout << "Welcome to probably the best racing adventure you will ever have!" << endl;
	cout << "You have " << LIVES << " lives to reach the finish line as fast as possible" << endl;
	cout << "Your supercar has a manual gearbox with " << TOP_GEAR << " gears" << endl;
	cout << "--- GOOD LUCK! ---" << endl << endl;
	cout << "Map Initialization Status -> COMPLETED" << endl;
	cout << "Game Status -> START" << endl;
}

Mesh * Homework2::CreateMesh(string name, glm::vec3 bottomLeftCorner, Road::BorderType borderType)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	if (name.compare(0, ROADPART_PREFIX.length(), ROADPART_PREFIX) == 0) {
		// Create vector and indices for a roadpart object
		vertices = RoadFactory::createRoadPartVertices(bottomLeftCorner);
		indices = RoadFactory::createRoadPartIndices();
	}
	else if (name.compare(0, BORDER_PREFIX.length(), BORDER_PREFIX) == 0) {
		// Create vector and indices for a border object
		vertices = RoadFactory::createBorderVertices(bottomLeftCorner, borderType);
		indices = RoadFactory::createCuboidIndices();

		// Add the border collision line
		Line line;
		line.x1 = RoadFactory::getBorderX1(bottomLeftCorner, borderType);
		line.y1 = RoadFactory::getBorderY1(bottomLeftCorner, borderType);
		line.x2 = RoadFactory::getBorderX2(bottomLeftCorner, borderType);
		line.y2 = RoadFactory::getBorderY2(bottomLeftCorner, borderType);;
		collisionBorderLines.push_back(line);
	}
	else if (name.compare(0, EARTH_PREFIX.length(), EARTH_PREFIX) == 0) {
		// Create vector and indices for the earth object
		vertices = EarthFactory::createEarthVertices();
		indices = RoadFactory::createCuboidIndices();
	}
	else if (name.compare(0, SKY_PREFIX.length(), SKY_PREFIX) == 0) {
		// Create vector and indices for the sky object
		vertices = SkyFactory::createSkyVertices();
		indices = RoadFactory::createCuboidIndices();
	}
	else if (name.compare(0, CAR_PREFIX.length(), CAR_PREFIX) == 0) {
		// Create vector and indices for the car object
		vertices = CarFactory::createCarVertices();
		indices = RoadFactory::createCuboidIndices();
	}

	// Create the VAO and bind it
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// Set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// Set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// Set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// Set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Homework2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Homework2::Update(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix;

	// Update the current hour of the 24h day
	hourOfTheDay = fmod(hourOfTheDay + deltaTimeSeconds * ONE_MINUTE, 24);

	// Render the earth and the sky using the custom shader
	RenderSimpleMesh(meshes[EARTH_PREFIX], shaders[SHADER_NAME], glm::mat4(1));
	RenderSimpleMesh(meshes[SKY_PREFIX], shaders[SHADER_NAME], glm::mat4(1));

	// Render the road using the custom shader and borders with generic shader
	for (int i = 0; i < roadPartsMeshes.size(); i++) {
		RenderSimpleMesh(roadPartsMeshes[i], shaders[SHADER_NAME], glm::mat4(1));
		RenderSimpleMesh(bordersMeshes[2 * i], shaders["VertexColor"], glm::mat4(1));
		RenderSimpleMesh(bordersMeshes[2 * i + 1], shaders["VertexColor"], glm::mat4(1));
	}

	// Render the next obstacle on the track
	for (int i = 0; i < obstacles.size(); i++) {
		modelMatrix = glm::translate(glm::mat4(1), glm::vec3(obsCoords[2 * i], 0, obsCoords[2 * i + 1]));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(0, 1, 0));
		RenderSimpleMesh(obstacles[i], shaders["VertexNormal"], modelMatrix);
	}

	// Update the angle of the movement
	angleOfMovement += (rotateLeft) ? deltaTimeSeconds * MOVEMENT_ROTATION : 0;
	angleOfMovement -= (rotateRight) ? deltaTimeSeconds * MOVEMENT_ROTATION : 0;

	// Calculate offset for car movement
	float moveOffset = currentGear * deltaTimeSeconds * SPEED;
	float moveOffsetX = currentGear * moveOffset * cos(M_PI / 2 + RADIANS(angleOfMovement));
	float moveOffsetZ = currentGear * -moveOffset * sin(M_PI / 2 + RADIANS(angleOfMovement));

	// Update camera
	{
		cameraX += moveOffsetX;
		cameraZ += moveOffsetZ;
		if (isCameraFPS) {
			// Calculate offset for FPS Camera to be exactly in front of the car
			float fpsOffsetX = Car::CAR_LENGTH * cos(M_PI / 2 + RADIANS(angleOfMovement));
			float fpsOffsetZ = -Car::CAR_LENGTH * sin(M_PI / 2 + RADIANS(angleOfMovement));

			// Set FPS Camera Properties
			GetSceneCamera()->SetPosition(glm::vec3(cameraX + fpsOffsetX, FPS_CAMERA_Y, cameraZ + fpsOffsetZ));
			GetSceneCamera()->SetRotation(glm::vec3(CAMERA_ROTATION_X, RADIANS(angleOfMovement), 0));
		}
		else {
			// Set TPS Camera Properties
			GetSceneCamera()->SetPosition(glm::vec3(cameraX, cameraY, cameraZ + TPS_CAMERA_Z));
			GetSceneCamera()->SetRotation(glm::vec3(CAMERA_ROTATION_X, RADIANS(angleOfMovement) / 3, 0));
		}
	}
	
	// Update the car coordinates
	{
		carCoords[CAR_INDEX] += moveOffsetX;
		carCoords[CAR_INDEX + 1] += moveOffsetZ;
	}

	// Render the car
	{
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(carCoords[CAR_INDEX], 0,
				carCoords[CAR_INDEX + 1]));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(angleOfMovement), glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes[CAR_PREFIX], shaders["VertexColor"], modelMatrix);
	}
	
	// Update tires rotation
	tireRotation -= deltaTimeSeconds * currentGear * TIRE_ROTATION;

	// Render the tires
	for (int i = 0; i < COUNT_TIRES; i++) {
		// Update tires coordinates
		carCoords[2 * i] += moveOffsetX;
		carCoords[2 * i + 1] += moveOffsetZ;

		// Calculate translation
		float translateX = carCoords[2 * i] - tireOffsets[2 * i];
		float translateZ = carCoords[2 * i + 1] - tireOffsets[2 * i + 1];

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(translateX, 0, translateZ));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(angleOfMovement), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(tireOffsets[2 * i], TIRE_RADIUS, tireOffsets[2 * i + 1]));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(tireRotation), glm::vec3(1, 0, 0));
		RenderSimpleMesh(meshes[TIRE_PREFIX + to_string(i)], shaders["VertexNormal"], modelMatrix);
	}

	// Calculate offsets for points in front of the car
	modelMatrix = glm::rotate(glm::mat4(1), RADIANS(angleOfMovement), glm::vec3(0, 1, 0));
	glm::vec3 offsetFrontCarLeft = modelMatrix * glm::vec4(-Car::CAR_WIDTH, 0, -Car::CAR_LENGTH, 1);
	glm::vec3 offsetFrontCarRight = modelMatrix * glm::vec4(Car::CAR_WIDTH, 0, -Car::CAR_LENGTH, 1);

	// Calculate front of the car
	Line frontOfTheCar;
	frontOfTheCar.x1 = carCoords[CAR_INDEX] + offsetFrontCarLeft[0];
	frontOfTheCar.y1 = carCoords[CAR_INDEX + 1] + offsetFrontCarLeft[2];
	frontOfTheCar.x2 = carCoords[CAR_INDEX] + offsetFrontCarRight[0];
	frontOfTheCar.y2 = carCoords[CAR_INDEX + 1] + offsetFrontCarRight[2];

	// Detect border collisions with the front of the car
	for (Line line : collisionBorderLines) {
		int collision = get_line_intersection(frontOfTheCar.x1, frontOfTheCar.y1, frontOfTheCar.x2,
			frontOfTheCar.y2, line.x1, line.y1, line.x2, line.y2, NULL, NULL);
		if (collision == 1) {
			if (currentGear != 0) {
				currentGear = 0;
				occurredCollisions++;
				cout << "Oops, You hit a BORDER and lost a life!" << endl;
				cout << "Lives left -> " << LIVES - occurredCollisions << endl;
				break;
			}
		}
	}

	// Detect collision with obstacles
	for (int i = 0; i < collisionCircles.size(); i++) {
		if (detectCollisionBetweenLineAndCircle(frontOfTheCar.x1, frontOfTheCar.y1,
			frontOfTheCar.x2, frontOfTheCar.y2, collisionCircles[i])) {

			// Remove the obstacle
			collisionCircles.erase(collisionCircles.begin() + i);
			obsCoords.erase(obsCoords.begin() + 2 * i);
			obsCoords.erase(obsCoords.begin() + 2 * i);
			obstacles.erase(obstacles.begin() + i);
			
			currentGear = 0;
			occurredCollisions++;
			cout << "Oops, You hit an OBSTACLE and lost a life!" << endl;
			cout << "Lives left -> " << LIVES - occurredCollisions << endl;
			break;
		}
	}

	// Detect crossing the finish line
	{
		int collision = get_line_intersection(frontOfTheCar.x1, frontOfTheCar.y1, frontOfTheCar.x2,
			frontOfTheCar.y2, finishLine.x1, finishLine.y1, finishLine.x2, finishLine.y2, NULL, NULL);
		if (collision == 1) {
			// WIN
			Engine::GetWindow()->Close();

			float time = Engine::GetElapsedTime();
			int lives = LIVES - occurredCollisions;
			if (time < bestTime || (time == bestTime && lives > bestLives)) {
				cout << "NEW HIGHSCORE!" << endl;
				
				// Update highscore file 
				ofstream oHighscoreFile(PATH_TO_HIGHSCORE_FILE);

				if (oHighscoreFile.is_open())
				{
					oHighscoreFile << time << " " << lives;
				}
				
				oHighscoreFile.close();
			}
			cout << "Congrats! You reached the finish line in " << time << " seconds";
			cout << " with " << lives << " lives left" << endl;
			cout << "Game Status -> WIN" << endl;
		}
	}
}

void Homework2::FrameEnd()
{
	DrawCoordinatSystem();

	if (occurredCollisions >= LIVES) {
		// DEFEAT
		Engine::GetWindow()->Close();
		cout << "FAIL! You lost (no more lives left) in " << Engine::GetElapsedTime() << " seconds";
		cout << endl << "Game Status -> DEFEAT" << endl;
	}
}

int Homework2::GetMeshSHID(Mesh *mesh)
{
	if (strncmp(mesh->GetMeshID(), ROADPART_PREFIX.c_str(), ROADPART_PREFIX.length()) == 0) {
		return ROADPART_SHID;
	}
	else if (strncmp(mesh->GetMeshID(), EARTH_PREFIX.c_str(), EARTH_PREFIX.length()) == 0) {
		return EARTH_SHID;
	}
	else {
		return SKY_SHID;
	}
}

void Homework2::RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Get shader location for uniform mat4 "Model"
	int modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");

	// Set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Get shader location for uniform mat4 "View"
	int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");

	// Set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Get shader location for uniform mat4 "Projection"
	int projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// Set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (strcmp(shader->GetName(), SHADER_NAME) == 0) {
		// Send the hour of the day to the shader
		int hour = glGetUniformLocation(shader->GetProgramID(), "hour");
		glUniform1f(hour, hourOfTheDay);

		// Send the ID of the mesh to the shader
		int id = glGetUniformLocation(shader->GetProgramID(), "id");
		glUniform1i(id, GetMeshSHID(mesh));
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Homework2::OnInputUpdate(float deltaTime, int mods)
{
}

void Homework2::OnKeyPress(int key, int mods)
{
	// Change gear, if possible
	if (key == GLFW_KEY_UP) {
		if (currentGear < TOP_GEAR) {
			currentGear++;
		}
	}
	else if (key == GLFW_KEY_DOWN) {
		if (currentGear > 0) {
			currentGear--;
		}
	}

	// Change direction
	if (key == GLFW_KEY_LEFT) {
		rotateLeft = true;
	} else if (key == GLFW_KEY_RIGHT) {
		rotateRight = true;
	}

	// Change the type of camera
	if (key == GLFW_KEY_F) {
		isCameraFPS = !isCameraFPS;
	}

	// Update draw mode
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_LINE:
			polygonMode = GL_FILL;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
		
}

void Homework2::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_LEFT) {
		rotateLeft = false;
	}
	else if (key == GLFW_KEY_RIGHT) {
		rotateRight = false;
	}
}

void Homework2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Homework2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Homework2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Homework2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework2::OnWindowResize(int width, int height)
{
}
