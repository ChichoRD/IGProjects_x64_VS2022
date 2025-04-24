#pragma once
#include "color_material_entity.h"

class Disk : public color_material_entity
{
public:
	Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, const glm::dvec4 color = glm::dvec4{ 1.0 });
};

