#include "scene_e6.h"
#include "rgb_triangle.h"

void scene_e6::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f);

	gObjects.push_back(new rgb_triangle());
}
