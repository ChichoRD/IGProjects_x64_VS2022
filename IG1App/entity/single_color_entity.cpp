#include "single_color_entity.h"

single_color_entity::single_color_entity(glm::dvec4 color)
	: color(color)
{
	mShader = Shader::get("simple");
}

void single_color_entity::render(const glm::mat4& modelViewMat) const
{
	(void)modelViewMat;
	if (mMesh != nullptr) {
		mShader->use();
		this->single_color_entity::upload_model(mModelMat);
		mShader->setUniform("color", glm::vec4(color));

		mMesh->render();
	}
}
