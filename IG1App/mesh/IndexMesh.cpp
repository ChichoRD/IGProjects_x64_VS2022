#include "IndexMesh.h"
#include <array>

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
	const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax = 2 * 3.141516f) {
	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	int tamPerfil = profile.size();
	mesh->vVertices.reserve(nSamples * tamPerfil);
	// Genera los vértices de las muestras
	GLdouble theta1 = 2 * 3.141516f / nSamples;
	for (int i = 0; i <= nSamples; ++i) { // muestra i-ésima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}

	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara
			if (profile[j].x != 0.0) // triángulo inferior
			{
				const std::array indices{
					glm::uvec2{i, j},
					glm::uvec2{i, j + 1 },
					glm::uvec2{i + 1, j}
				};
				for (auto p : indices) {
					mesh->vIndexes.push_back(p.x * tamPerfil + p.y);
				}
			}
			if (profile[j + 1].x != 0.0) // triángulo superior
			{
				const std::array indices{
					glm::uvec2{i, j + 1},
					glm::uvec2{i + 1, j + 1 },
					glm::uvec2{i + 1, j}
				};

				for (auto p : indices) {
					mesh->vIndexes.push_back(p.x * tamPerfil + p.y);
				}
			}
		}
	mesh->mNumVertices = mesh->vVertices.size();
	return mesh;
}


void IndexMesh::draw() const
{
	glDrawElements(
		mPrimitive, // primitiva ( GL_TRIANGLES , etc.)
		vIndexes.size(), // número de índices
		GL_UNSIGNED_INT, // tipo de los índices
		nullptr // offset en el VBO de índices
	);
}
