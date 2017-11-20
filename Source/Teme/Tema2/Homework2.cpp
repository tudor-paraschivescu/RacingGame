#include "Homework2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

glm::vec3 centerOfMovingObject = glm::vec3(0, 0, 0);
vector<Mesh *> roadPartsMeshes;
vector<Mesh *> bordersMeshes;

Homework2::Homework2()
{
}

Homework2::~Homework2()
{
}

void Homework2::Init()
{
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
		do
		{
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
	
	// Create a vector that will contain the meshes for all road parts
	float currX = ROAD_START_X;
	float currZ = ROAD_START_Z;
	int indexRoadPart = 0;

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

	// Create a shader program for drawing face polygon with the color of the normal
	Shader *shader = new Shader("ShaderHomework2");
	shader->AddShader("Source/Teme/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
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
		indices = RoadFactory::createBorderIndices();
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

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Homework2::Update(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	/*
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
	RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], modelMatrix);
	*/
	//RenderSimpleMesh(meshes["bamboo"], shaders["ShaderHomework2"], modelMatrix);

	for (int i = 0; i < roadPartsMeshes.size() && i < ROADBLOCKS_TO_RENDER; i++) {
		RenderSimpleMesh(roadPartsMeshes[i], shaders["VertexColor"], modelMatrix);
		RenderSimpleMesh(bordersMeshes[2 * i], shaders["VertexColor"], modelMatrix);
		RenderSimpleMesh(bordersMeshes[2 * i + 1], shaders["VertexColor"], modelMatrix);
	}
}

void Homework2::FrameEnd()
{
	DrawCoordinatSystem();
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

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Homework2::OnInputUpdate(float deltaTime, int mods)
{
}

void Homework2::OnKeyPress(int key, int mods)
{
	// Example of recycling road parts and borders
	if (roadPartsMeshes.size() != 0)
	{
		roadPartsMeshes.erase(roadPartsMeshes.begin());
		bordersMeshes.erase(bordersMeshes.begin());
		bordersMeshes.erase(bordersMeshes.begin());
	}
}

void Homework2::OnKeyRelease(int key, int mods)
{
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
