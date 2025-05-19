#include "tie_fighter_wing.h"
#include "glm/ext/scalar_constants.hpp"
#include "IndexMesh.h"
#include "Cone.h"
#include "IG1App.h"
#include <algorithm>

tie_fighter_wing::tie_fighter_wing() : entity_with_texture("../assets/images/noche.jpg", true) {
	mMesh = new mesh_uv{ mesh_uv::generate_tie_wings(100.0f, 80.0f, 50.0f) };
	mMesh->colors().resize(mMesh->vertices().size(), glm::vec4{ 1.0f, 1.0f, 1.0f, 0.5f });
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
