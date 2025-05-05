#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Light.h" 

#include <vector>
#include <memory>

class Scene
{
public:
	constexpr static const GLdouble axis_unit_size = 400.0;
	constexpr static const glm::dvec4 cornflower_blue { 0.6f, 0.7f, 0.8f, 1.0f };
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();
	virtual void destroy();

	void render(Camera const& cam) const;
	virtual void update(double time_seconds, double delta_time_seconds);

	// load or unload scene data into the GPU
	void load();
	void unload();

	inline std::vector<std::unique_ptr<Light>> get_lights() { return lights; }

protected:
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<std::unique_ptr<Light>> lights;
};

template <typename LightType>
bool upload_light_as(Shader &shader, const Light& light, const Camera& camera) {
	const LightType *directional_light = dynamic_cast<const LightType *>(&light);
	if (directional_light != nullptr) {
		directional_light->upload(shader, camera.viewMat());
		return true;
	}
	return false;
}

#endif //_H_Scene_H_
