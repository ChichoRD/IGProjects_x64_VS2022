#include "Disk.h"
#include "IndexMesh.h"

Disk::Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples, const glm::dvec4 color) : color_material_entity(color)
{
	std::vector<glm::vec2> disk_profile{ nRings };

	for (int c = 0; c < nRings; c++) {
		GLdouble t = (GLdouble)c / (GLdouble)nRings;
		disk_profile[c] = glm::vec2((R - r) * t + r, 0);
	}

	mMesh = IndexMesh::generateByRevolution(disk_profile, nSamples);
	load();
}