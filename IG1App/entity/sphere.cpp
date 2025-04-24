#include "sphere.h"
#include "IndexMesh.h"
#include <glm/ext/scalar_constants.hpp>

sphere::sphere(const GLdouble radius, const GLuint slice_point_count, const GLuint slice_count, const glm::dvec4 color)
	: color_material_entity{color} {
	std::vector<glm::vec2> half_circle_profile{ slice_point_count + 1};
	for (size_t i = 0; i <= slice_point_count; ++i) {
		const float angle_normalized = float(i) / float(slice_point_count) - 0.5f;
		const float angle = angle_normalized * glm::pi<float>();
		half_circle_profile.at(i) = glm::vec2{ radius * glm::cos(angle), radius * glm::sin(angle) };
	}
	mMesh = IndexMesh::generateByRevolution(half_circle_profile, slice_count, glm::pi<float>() * 2.0f);
	mMesh->load();
	generate_and_load_normals_debug_mesh(glm::dvec4{ 1.0f, 0.94 * 0.25f, 0.0, 1.0 });
}
