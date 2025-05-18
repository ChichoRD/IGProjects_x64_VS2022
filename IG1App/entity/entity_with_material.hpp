#pragma once
#include "Entity.h"
#include "Material.h"
class entity_with_material :
    public Abs_Entity
{
    Material material;
	Mesh normals_debug_mesh;
	glm::dvec4 normal_debug_color;

	static bool debug_normals_enabled;
public:

	entity_with_material(const Material material);
	static void set_debug_normals_enabled(const bool enable);
	static bool get_debug_normals_enabled();
	void render(const glm::mat4 &basis) const override;

	Material& get_material();
	const Material& get_material() const;

protected:
	size_t generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color);

};

