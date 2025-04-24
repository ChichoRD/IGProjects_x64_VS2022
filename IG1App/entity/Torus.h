#pragma once
#include "color_material_entity.h"

class Torus : public color_material_entity
{
public:
	Torus(GLdouble R, GLdouble radius, GLuint nPoints = 40, GLuint nSamples = 40);
};
