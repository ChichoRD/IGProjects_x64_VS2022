#include "IndexMesh.h"

void IndexMesh::load() {
	Mesh::load(); glBindVertexArray(mVAO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

IndexMesh* IndexMesh::generateByRevolution(
	const std::vector<glm::vec2>& profile, GLuint nSamples,
	GLfloat angleMax = 2 * 3.141516f) {

}