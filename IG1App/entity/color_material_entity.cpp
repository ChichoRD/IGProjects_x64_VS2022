#include "color_material_entity.h"

bool color_material_entity::debug_normals_enabled = false;

color_material_entity::color_material_entity(const glm::dvec4 color)
	: single_color_entity{ color }, normals_debug_mesh{}, normal_debug_color{1.0f} {
	mShader = Shader::get("simple_light");
}

void color_material_entity::render(const glm::mat4& basis) const {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	single_color_entity::render(basis);
	if (debug_normals_enabled) {
		Shader& debug_shader = *Shader::get("simple");
		debug_shader.use();
		debug_shader.setUniform("model", basis * mModelMat);
		debug_shader.setUniform("color", glm::vec4(normal_debug_color));

		normals_debug_mesh.render();
	}

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
}

size_t color_material_entity::generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color) {
	normals_debug_mesh.unload();
	normal_debug_color = debug_color;

	normals_debug_mesh.set_primitive(GL_LINES);
	normals_debug_mesh.vertices().reserve(mMesh->vertices().size() << 1);

	for (size_t i = 0; i < mMesh->vertices().size(); i++) {
		const glm::vec3 vertex = mMesh->vertices().at(i);
		const glm::vec3 normal = mMesh->normals().at(i);
		normals_debug_mesh.vertices().push_back(vertex);

		constexpr static const float scale = 75.0f;
		normals_debug_mesh.vertices().push_back(vertex + normal * scale);
	}
	normals_debug_mesh.set_vertex_range(normals_debug_mesh.vertices().size());
	normals_debug_mesh.load();
	return mMesh->size();
}
