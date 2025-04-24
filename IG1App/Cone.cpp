#include "Cone.h"
#include "IndexMesh.h"
#include "glm/ext/scalar_constants.hpp"

Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::dvec4 color) : color_material_entity(color)
{
	std::vector<glm::vec2> cone_profile{ nRings };

	for (int c = 0; c < nRings; c++) {
		GLdouble t = c / nRings;
		cone_profile[c] = glm::vec2((R - r) * t + r, h * c/t);
	}

	mMesh = IndexMesh::generateByRevolution(cone_profile, nSamples);
}