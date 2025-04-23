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

void IndexMesh::unload() {

}

IndexMesh* IndexMesh::generateByRevolution(
	const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax) {
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

IndexMesh* IndexMesh::generate_indexed_box(const GLdouble side_length) {
	const float half_side = float(side_length) / 2.0f;
	const std::array<glm::vec3, 8> vertices{
		glm::vec3(-half_side, -half_side, -half_side),
		glm::vec3(half_side, -half_side, -half_side),
		glm::vec3(half_side,  half_side, -half_side),
		glm::vec3(-half_side,  half_side, -half_side),
		glm::vec3(-half_side, -half_side,  half_side),
		glm::vec3(half_side, -half_side,  half_side),
		glm::vec3(half_side,  half_side,  half_side),
		glm::vec3(-half_side,  half_side,  half_side)
	};
	const std::vector<glm::vec4> colors{ vertices.size() };

	static const std::array<glm::vec3, 8> normals{
		glm::normalize(glm::vec3(-1.0, -1.0, -1.0)),
		glm::normalize(glm::vec3(1.0, -1.0, -1.0)),
		glm::normalize(glm::vec3(1.0,  1.0, -1.0)),
		glm::normalize(glm::vec3(-1.0,  1.0, -1.0)),
		glm::normalize(glm::vec3(-1.0, -1.0,  1.0)),
		glm::normalize(glm::vec3(1.0, -1.0,  1.0)),
		glm::normalize(glm::vec3(1.0,  1.0,  1.0)),
		glm::normalize(glm::vec3(-1.0,  1.0,  1.0))
	};
	const std::array<GLuint, 36> indices{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		0, 1, 5, 0, 5, 4,
		2, 3, 7, 2, 7, 6,
		0, 3, 7, 0, 7, 4,
		1, 2, 6, 1, 6, 5
	};
	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->vVertices.reserve(vertices.size());
	mesh->vNormals.reserve(normals.size());
	mesh->vIndexes.reserve(indices.size());

	mesh->vVertices.insert(mesh->vVertices.end(), vertices.begin(), vertices.end());
	mesh->vNormals.insert(mesh->vNormals.end(), normals.begin(), normals.end());
	mesh->vIndexes.insert(mesh->vIndexes.end(), indices.begin(), indices.end());
	
	mesh->vColors = std::move(colors);
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
