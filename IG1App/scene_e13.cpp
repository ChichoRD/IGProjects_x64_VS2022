#include "scene_e13.h"
#include "rgb_triangle.h"
#include "Cube.h"

#include <glm/gtc/matrix_transform.hpp>

void scene_e13::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	gObjects.push_back(new Cube());
}
