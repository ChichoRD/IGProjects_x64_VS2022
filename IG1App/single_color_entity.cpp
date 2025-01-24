#include "single_color_entity.h"

single_color_entity::single_color_entity(glm::dvec4 color)
	: color(color)
{
	mShader = Shader::get("simple");
}

void single_color_entity::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glm::mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		this->single_color_entity::upload(aMat);
		mMesh->render();
	}
}

void single_color_entity::upload(const glm::mat4& mModelViewMat) const
{
	Abs_Entity::upload(mModelViewMat);
	mShader->setUniform("color", glm::vec4(color));
}
