#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(axis_unit_size));

	std::unique_ptr<DirLight> directional_light{
		std::make_unique<DirLight>(static_cast<int>(lights.size()))
	};
	directional_light->setDirection(glm::normalize(glm::vec3{ -1.0, -1.0, -1.0 }));
	lights.push_back(std::move(directional_light));
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects) {
		delete el;
	}

	lights.clear();
	gObjects.clear();
}

void
Scene::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();
}

void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(1.0, 1.0, 1.0, 1.0); // background color (alpha=1 -> opaque)

	glEnable(GL_DEPTH_TEST);          // enable Depth test
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

static size_t upload_lights(const std::vector<std::unique_ptr<Light>>& lights, const Camera &camera) {
	static Shader& lights_shader = *Shader::get("light");
	constexpr static const size_t max_directional_lights = 2;
	constexpr static const size_t max_point_lights = 4;
	constexpr static const size_t max_spot_lights = 4;

	size_t directional_light_count = 0;
	size_t point_light_count = 0;
	size_t spot_light_count = 0;
	lights_shader.use();
	for (const std::unique_ptr<Light> &light : lights) {
		if (directional_light_count < max_directional_lights && upload_light_as<DirLight>(lights_shader, *light, camera)) {
			directional_light_count++;
		} else if (point_light_count < max_point_lights && upload_light_as<PosLight>(lights_shader, *light, camera)) {
			point_light_count++;
		} else if (spot_light_count < max_spot_lights && upload_light_as<SpotLight>(lights_shader, *light, camera)) {
			spot_light_count++;
		}
	}
	return directional_light_count + point_light_count + spot_light_count;
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();
	const size_t uploaded_count = upload_lights(lights, cam);
	if (uploaded_count < lights.size()) {
		std::cerr << "Warning: not all lights were uploaded. "
			<< "Uploaded: " << uploaded_count
			<< ", Lights: " << lights.size() << std::endl;
	}

	for (Abs_Entity* el : gObjects)
		el->render(glm::identity<glm::mat4>());
}

void Scene::update(double time_seconds, double delta_time_seconds)
{
	for (Abs_Entity* el : gObjects)
		el->update(time_seconds, delta_time_seconds);
}
