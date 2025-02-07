#include "scene_e13.h"
#include "rgb_triangle.h"

#include <glm/gtc/matrix_transform.hpp>

void scene_e13::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	auto tri = new rgb_triangle();
	tri->setModelMat(glm::translate(tri->modelMat(), { 0.5f * 400.0f , 0.0f, 0.0f}));
	gObjects.push_back(tri);
}
