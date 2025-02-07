#include "rgb_triangle.h"

rgb_triangle::rgb_triangle()
	: EntityWithColors()
{
	mMesh = Mesh::generate_regular_polygon(3, 200, std::vector{
		glm::vec4(1,0,0,1),
		glm::vec4(0,1,0,1),
		glm::vec4(0,0,1,1)
		}, GL_TRIANGLES);
	load();
}

void rgb_triangle::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		glm::mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);
		{
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			mMesh->render();

			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();
		}
		glDisable(GL_CULL_FACE);
	}
}