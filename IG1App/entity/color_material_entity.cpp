#include "color_material_entity.h"

color_material_entity::color_material_entity(const glm::dvec4 color)
	: single_color_entity{ color }, normals_debug_mesh{}, normal_debug_color{1.0f} {
	mShader = Shader::get("simple_light");
}

size_t color_material_entity::generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color) {
	normals_debug_mesh.unload();

	normals_debug_mesh.set_primitive(GL_LINES);
	normals_debug_mesh.vertices().reserve(mMesh->vertices().size() << 1);

	for (size_t i = 0; i < mMesh->vertices().size(); i++) {
		const glm::vec3 vertex = mMesh->vertices().at(i);
		const glm::vec3 normal = mMesh->normals().at(i);
		normals_debug_mesh.vertices().push_back(vertex);

		constexpr static const float scale = 75.0f;
		normals_debug_mesh.vertices().push_back(vertex + normal * scale);
	}

	normals_debug_mesh.load();
	return mMesh->size();
}
