#pragma once
#include "Mesh.h"
#include <vector>

class IndexMesh : public Mesh
{
public:
	void draw() const override;
	void load() override;
	void unload() override;

	static IndexMesh* generateByRevolution(
		const std::vector<glm::vec2>& profile, GLuint nSamples,
		GLfloat angleMax = 2 * 3.141516f);
	static IndexMesh* generate_indexed_box(const GLdouble side_length);
private:
	std::vector<GLuint> vIndexes;
	GLuint mIBO;
};

