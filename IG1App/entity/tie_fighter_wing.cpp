#include "tie_fighter_wing.h"
#include "glm/ext/scalar_constants.hpp"
#include "IndexMesh.h"
#include "Cone.h"
#include "IG1App.h"
#include <algorithm>

tie_fighter_wing::tie_fighter_wing() : entity_with_texture("../assets/images/noche.jpg", true) {
	std::vector<glm::vec2> cone_profile{ 4 };

	//100, 150, 150, 4, 5, Scene::cornflower_blue, glm::half_pi<GLfloat>() + glm::pi<GLfloat>() / 6
	GLdouble h = 100;
	GLdouble r = 150, R = 150;
	GLuint nRings = 4;
	GLuint nSamples = 5;
	static const constexpr GLfloat angleMax = 2 * glm::pi<GLfloat>()/3;

	for (GLuint c = 0; c < nRings; c++) {
		GLdouble t = (GLdouble)c / (GLdouble)nRings;
		cone_profile[c] = glm::vec2((R - r) * t + r, h * t);
	}

	mMesh = IndexMesh::generate_by_revolution_no_cap(cone_profile, nSamples, angleMax);

    load();
}

void tie_fighter_wing::render(const glm::mat4& basis) const {
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	render_with_texture_and_model(texture, basis * mModelMat);

	// glDepthMask(GL_TRUE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void tie_fighter_wing::update(double time_seconds, double delta_time_seconds) {
	(void)time_seconds;
	(void)delta_time_seconds;
	constexpr static const auto camera_depth =
		[](const glm::mat4 model_view_matrix, const glm::vec3 position) {
		return (model_view_matrix * glm::vec4{ position, 1.0f }).z;
		};

	const glm::mat4 model_view_matrix = glm::mat4{ IG1App::s_ig1app.camera().viewMat() } *mModelMat;
	std::sort(mMesh->vertices().begin(), mMesh->vertices().end(), [model_view_matrix](const glm::vec3& a, const glm::vec3& b) {
		return camera_depth(model_view_matrix, a) < camera_depth(model_view_matrix, b);
		});
}
