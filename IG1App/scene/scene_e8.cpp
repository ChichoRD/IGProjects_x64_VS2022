#include "scene_e8.h"
#include "rgb_rectangle.h"

void scene_e8::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	gObjects.push_back(new rgb_rectangle(0.8f * 400.0f, 0.5f * 400.f, {
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
	}));
}
