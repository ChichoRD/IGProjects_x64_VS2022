#include "Cone.h"
#include "IndexMesh.h"

Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples, const glm::dvec4 color, GLfloat angleMax) : color_material_entity(color)
{
	std::vector<glm::vec2> cone_profile{ nRings };

	for (int c = 0; c < nRings; c++) {
		GLdouble t = (GLdouble)c / (GLdouble)nRings;
		cone_profile[c] = glm::vec2((R - r) * t + r, h * t);
	}
	IndexMesh *(*funcs[])(const std::vector<glm::vec2> &profile, GLuint nSamples, GLfloat angleMax) = {
		IndexMesh::generateByRevolution,
		IndexMesh::generate_by_revolution_no_cap
	};
	mMesh = funcs[angleMax != 2 * glm::pi<GLfloat>()](cone_profile, nSamples, angleMax);
	load();
}