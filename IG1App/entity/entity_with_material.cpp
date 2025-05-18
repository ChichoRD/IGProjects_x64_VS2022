#include "entity_with_material.hpp"

entity_with_material::entity_with_material(const Material material)
	: Abs_Entity(), material(material), normals_debug_mesh{}, normal_debug_color{} {
	mShader = Shader::get("light");
}

void entity_with_material::set_debug_normals_enabled(const bool enable) {
	debug_normals_enabled = enable;
}

bool entity_with_material::get_debug_normals_enabled() {
	return debug_normals_enabled;
}

void entity_with_material::render(const glm::mat4& basis) const {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	if (mMesh != nullptr) {
		mShader->use();
		upload_model(basis * mModelMat);
		material.upload(*mShader);

		mMesh->render();
	}

	if (debug_normals_enabled && normals_debug_mesh.is_loaded()) {
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

size_t entity_with_material::generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color) {
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