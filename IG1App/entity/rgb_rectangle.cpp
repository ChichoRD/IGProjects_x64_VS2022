#include "rgb_rectangle.h"
#include "Mesh.h"

rgb_rectangle::rgb_rectangle(GLdouble w, GLdouble h, std::array<glm::vec4, 4>&& vertex_colours)
{
	mMesh = Mesh::generate_rectangle(w, h, std::move(vertex_colours));
	load();
}

void rgb_rectangle::render(const glm::mat4& modelViewMat) const
{
	(void)modelViewMat;
	if (mMesh != nullptr) {
		mShader->use();
		this->EntityWithColors::upload_model(mModelMat);

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