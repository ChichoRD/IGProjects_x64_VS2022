#pragma once
#include "color_material_entity.h"

class indexed_box :
    public color_material_entity
{
public:
    indexed_box(const GLdouble side_length, const glm::dvec4 color = glm::dvec4{ 1.0 });
};

