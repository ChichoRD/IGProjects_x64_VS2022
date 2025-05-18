#include "color_material_entity.h"

bool color_material_entity::debug_normals_enabled = false;

color_material_entity::color_material_entity(const glm::dvec4 color)
	: entity_with_material{ Material{
		color
	} } {
}
