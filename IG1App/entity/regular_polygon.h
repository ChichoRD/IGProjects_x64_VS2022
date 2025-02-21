#pragma once
#include "single_color_entity.h"
class regular_polygon :
    public single_color_entity
{
public:
    regular_polygon(size_t sides, double radius = 1.0, glm::dvec4 color = glm::dvec4(1.0f));
    //virtual ~regular_polygon();
};

