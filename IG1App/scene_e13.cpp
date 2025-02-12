#include "scene_e13.h"
#include "rgb_triangle.h"
#include "RGB_Cube.h"

#include <glm/gtc/matrix_transform.hpp>

void scene_e13::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	auto cube = new RGB_Cube(300);
	gObjects.push_back(cube);
}
