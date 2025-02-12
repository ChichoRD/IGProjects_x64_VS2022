#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

class Scene
{
public:
	constexpr static const GLdouble axis_unit_size = 400.0;
	Scene() = default;
	~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();
	void destroy();

	void render(Camera const& cam) const;
	virtual void update(double time_seconds, double delta_time_seconds);

	// load or unload scene data into the GPU
	void load();
	void unload();

protected:
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
};

#endif //_H_Scene_H_
