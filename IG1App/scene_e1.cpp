#include "scene_e1.h"
#include "regular_polygon.h"

void scene_e1::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	// hahahhahsdhadsh!!!! please normalise the sizes!! i've been seeking why it doesn't render for 4 hours
	// it was rendering but it was too small to seeeeeeeeeee
	gObjects.push_back(new regular_polygon(5, 0.5 * 400.0, glm::dvec4(1.0f, 0.0f, 0.0f, 1.0f)));
	gObjects.push_back(new regular_polygon(3, 0.5 * 400.0, glm::dvec4(0.0f, 1.0f, 1.0f, 1.0f)));
	gObjects.push_back(new regular_polygon(256, 0.5 * 400.0, glm::dvec4(1.0f, 0.0f, 1.0f, 1.0f)));
}
