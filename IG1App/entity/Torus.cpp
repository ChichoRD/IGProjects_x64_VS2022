#include "Torus.h"
#include "IndexMesh.h"
#include "glm/ext/scalar_constants.hpp"

Torus::Torus(GLdouble R, GLdouble radius, GLuint nPoints, GLuint nSamples) {

	std::vector<glm::vec2> circle_profile{ nPoints };
	glm::vec2 centre{ R, 0.0f };

	for (size_t i = 0; i < circle_profile.size(); ++i) {
		const float angle_normalized = float(i) / float(circle_profile.size());
		const float angle = angle_normalized * glm::pi<float>() * 2;

		circle_profile.at(i) = centre + glm::vec2{ glm::cos(angle), glm::sin(angle) } * (float)radius;
	}

	mMesh = IndexMesh::generateByRevolution(circle_profile, nSamples);
	load();
}