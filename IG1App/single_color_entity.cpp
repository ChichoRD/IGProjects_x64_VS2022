#include "single_color_entity.h"

single_color_entity::single_color_entity(glm::dvec4 color)
	: Abs_Entity(), color(color)
{
	mShader = Shader::get("simple");
}

void single_color_entity::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glm::mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->setUniform("color", glm::vec4(color));
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}
