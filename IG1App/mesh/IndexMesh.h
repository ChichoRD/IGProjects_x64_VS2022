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
		GLfloat angleMax = 2 * 3.141516f, bool closes = true);
	static IndexMesh* generate_indexed_box(const GLdouble side_length);
	static std::vector<glm::vec3> normals_from_newell_indexed(
		const std::vector<glm::vec3>& vertices,
		const std::vector<GLuint>& indexes
	);
private:
	std::vector<GLuint> vIndexes;
	GLuint mIBO;
};

