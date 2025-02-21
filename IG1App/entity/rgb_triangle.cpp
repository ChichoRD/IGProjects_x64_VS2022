#include "rgb_triangle.h"

#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.hpp>

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
		upload_model_view(aMat);

		glEnable(GL_CULL_FACE);
		{
			glCullFace(GL_FRONT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			mMesh->render();

			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			mMesh->render();
		}
		glDisable(GL_CULL_FACE);
	}
}

void rgb_triangle::update(double time_seconds, double delta_time_seconds)
{
	(void)time_seconds;
	(void)delta_time_seconds;

	constexpr const float rotation_delta = glm::two_pi<float>() / 360.0f * 20.0f;
	constexpr const float translation_delta = glm::two_pi<float>() / 360.0f * 20.0f;

	glm::vec4 position = mModelMat[3];
	position = glm::rotate(glm::mat4(1), translation_delta, glm::vec3(0, 0, 1)) * position;
	position[3] = 1;

	mModelMat = mModelMat * glm::rotate(glm::mat4(1), rotation_delta, glm::vec3(0, 0, 1));
	mModelMat[3] = position;
}
