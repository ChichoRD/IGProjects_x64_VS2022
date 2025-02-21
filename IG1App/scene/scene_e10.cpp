#include "scene_e10.h"
#include "regular_polygon.h"
#include "rgb_triangle.h"
#include "rgb_rectangle.h"

#include "glm/ext/matrix_transform.hpp"

void scene_e10::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f);

	gObjects.push_back(new regular_polygon(256, 200));
	gObjects.push_back(new rgb_triangle());
	auto rect = new rgb_rectangle(400.0f, 200.f, {
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
		});
	gObjects.push_back(rect);
}