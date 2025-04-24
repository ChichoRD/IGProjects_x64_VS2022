#include "Torus.h"
#include "IndexMesh.h"
#include "glm/ext/scalar_constants.hpp"

Torus::Torus(GLdouble R, GLdouble radius, GLuint nPoints, GLuint nSamples)
	: color_material_entity{ glm::dvec4{ 0.1, 0.9, 0.1, 1.0 } } {

	std::vector<glm::vec2> circle_profile{ nPoints };
	glm::vec2 centre{ R, 0.0f };

	for (size_t i = 0; i < circle_profile.size(); ++i) {
		const float angle_normalized = float(i) / float(circle_profile.size());
		const float angle = angle_normalized * glm::pi<float>() * 2;

		circle_profile.at(i) = centre + glm::vec2{ glm::cos(angle), glm::sin(angle) } *(float)radius;
	}

	mMesh = IndexMesh::generateByRevolution(circle_profile, nSamples);
	load();
	generate_and_load_normals_debug_mesh(glm::dvec4{ 1.0, 0.94, 0.0, 1.0 });
}