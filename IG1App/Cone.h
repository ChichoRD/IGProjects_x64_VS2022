#pragma once
#include "color_material_entity.h"
#include "glm/ext/scalar_constants.hpp"

class Cone : public color_material_entity
{
public:
	Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::dvec4 color = glm::dvec4{ 1.0 }, GLfloat angleMax = 2 * glm::pi<GLfloat>());
};

