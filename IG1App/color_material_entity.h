#pragma once
#include "single_color_entity.h"
class color_material_entity :
    public single_color_entity
{
    color_material_entity(glm::dvec4 color = glm::dvec4{ 1.0f });
};

