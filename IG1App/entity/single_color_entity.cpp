#include "single_color_entity.h"

single_color_entity::single_color_entity(glm::dvec4 color)
	: color(color)
{
	mShader = Shader::get("simple");
}

[[maybe_unused]]
glm::dvec4 single_color_entity::get_color() const { return color; }

[[maybe_unused]]
void single_color_entity::set_color(glm::dvec4 c) { color = c; }

void single_color_entity::render(const glm::mat4& basis) const
{
	if (mMesh != nullptr) {
		mShader->use();
		this->single_color_entity::upload_model(basis * mModelMat);
		mShader->setUniform("color", glm::vec4(color));

		mMesh->render();
	}
}
