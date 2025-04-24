#pragma once
#include "color_material_entity.h"
class sphere :
    public color_material_entity
{
public:
	sphere(const GLdouble radius, const GLuint slice_point_count, const GLuint slice_count, const glm::dvec4 color);
};

