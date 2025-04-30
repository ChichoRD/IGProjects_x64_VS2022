#include "entity_with_material.hpp"

void entity_with_material::render(const glm::mat4& basis) const {
	if (mMesh != nullptr) {
		mShader->use();
		upload_model(basis * mModelMat);
		material.upload(*mShader);

		mMesh->render();
	}
}
