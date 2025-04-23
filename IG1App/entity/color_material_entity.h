#pragma once
#include "single_color_entity.h"
class color_material_entity :
    public single_color_entity
{
    Mesh normals_debug_mesh;
    glm::dvec4 normal_debug_color;
public:
    color_material_entity(const glm::dvec4 color = glm::dvec4{ 1.0f });

protected:
	size_t generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color);
};

