#pragma once
#include "color_material_entity.h"

class Sphere : public color_material_entity
{
	Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians, const glm::dvec4 color = glm::dvec4{ 1.0 });
};

