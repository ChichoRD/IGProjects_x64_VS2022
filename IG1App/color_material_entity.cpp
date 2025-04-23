#include "color_material_entity.h"

color_material_entity::color_material_entity(const glm::dvec4 color)
	: single_color_entity{color} {
	mShader = Shader::get("simple_light");
}
