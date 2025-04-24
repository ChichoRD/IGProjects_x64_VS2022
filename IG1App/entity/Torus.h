#pragma once
#include "single_color_entity.h"

class Torus : public single_color_entity
{
public:
	Torus(GLdouble R, GLdouble radius, GLuint nPoints = 40, GLuint nSamples = 40);
};
