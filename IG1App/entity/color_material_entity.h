#pragma once
#include "entity_with_material.hpp"
class color_material_entity :
    public entity_with_material
{
public:
    color_material_entity(const glm::dvec4 color = glm::dvec4{ 1.0f });
};

