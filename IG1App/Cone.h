#pragma once
#include "color_material_entity.h"
class Cone : public color_material_entity
{
public:
	Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::dvec4 color = glm::dvec4{ 1.0 }, GLfloat angleMax = 2 * 3.141516f);
};

