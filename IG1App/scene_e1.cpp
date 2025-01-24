#include "scene_e1.h"
#include "single_color_entity.h"

void scene_e1::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	gObjects.push_back(new single_color_entity(glm::dvec4(1.0f, 0.0f, 1.0f, 1.0f)));
}
