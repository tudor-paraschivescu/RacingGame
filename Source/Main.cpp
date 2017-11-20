#include <ctime>
#include <iostream>

using namespace std;

#include <Core/Engine.h>

#include <Teme\HomeworkList.h>

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	// Create a window property structure
	WindowProperties wp;
	wp.resolution = glm::ivec2(Homework1::WINDOW_WIDTH, Homework1::WINDOW_HEIGHT);
	wp.name = "Racing Game";

	// Init the Engine and create a new window with the defined properties
	WindowObject* window = Engine::Init(wp);

	// Create a new 2D world and start running it
	World *world = new Homework2();
	world->Init();
	world->Run();

	// Signals to the Engine to release the OpenGL context
	Engine::Exit();

	return 0;
}