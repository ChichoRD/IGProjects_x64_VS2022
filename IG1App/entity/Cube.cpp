#include "Cube.h"
using namespace glm;

Cube::Cube()
{
	mMesh = Mesh::generateCube(300);
	load();
}

void
Cube::render(mat4 const& modelViewMat) const
{
    (void)modelViewMat;
    if (mMesh != nullptr) {
        mShader->use();
        upload_model(mModelMat);

        glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        mMesh->render();

        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        mMesh->render();

        glDisable(GL_CULL_FACE);
    }
}
