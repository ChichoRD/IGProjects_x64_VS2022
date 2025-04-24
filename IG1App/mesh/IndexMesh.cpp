#include "IndexMesh.h"
#include <array>

void IndexMesh::load() {
	Mesh::load(); glBindVertexArray(mVAO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
}

void IndexMesh::unload() {
	Mesh::unload();
	glDeleteBuffers(1, &mIBO);
	mIBO = GL_NONE;
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
	const GLdouble half_side = side_length / 2.0;

	// 8 vertices CCW
	std::vector positions{
		glm::vec3(-half_side, -half_side, -half_side),
		glm::vec3(half_side, -half_side, -half_side),
		glm::vec3(half_side,  half_side, -half_side),
		glm::vec3(-half_side,  half_side, -half_side),
		glm::vec3(-half_side, -half_side,  half_side),
		glm::vec3(half_side, -half_side,  half_side),
		glm::vec3(half_side,  half_side,  half_side),
		glm::vec3(-half_side,  half_side,  half_side)
	};
	std::vector normals{
		glm::normalize(positions.at(0)),
		glm::normalize(positions.at(1)),
		glm::normalize(positions.at(2)),
		glm::normalize(positions.at(3)),
		glm::normalize(positions.at(4)),
		glm::normalize(positions.at(5)),
		glm::normalize(positions.at(6)),
		glm::normalize(positions.at(7))
	};
	std::vector<glm::vec4> colors{ positions.size(), glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f } };
	assert(positions.size() == normals.size());
	assert(positions.size() == colors.size());

	//CCW
	std::vector<GLuint> indices{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		0, 1, 5,
		0, 5, 4,

		1, 2, 6,
		1, 6, 5,

		2, 3, 7,
		2, 7, 6,

		3, 0, 4,
		3, 4, 7,

		0, 3, 2,
		0, 2, 1,
	};

	IndexMesh* mesh = new IndexMesh;

	mesh->mPrimitive = GL_TRIANGLES;
	mesh->vVertices = std::move(positions);
	mesh->vNormals = std::move(normals);
	mesh->vColors = std::move(colors);
	mesh->vIndexes = std::move(indices);
	mesh->mNumVertices = mesh->vVertices.size();

	return mesh;
}

std::vector<glm::vec3> IndexMesh::normals_from_newell_indexed(
	const std::vector<glm::vec3>& vertices,
	const std::vector<GLuint>& indexes
) {
	std::vector<glm::vec3> normals(vertices.size());
	for (size_t i = 0; i < indexes.size(); i += 3) {
		const glm::vec3& v0 = vertices[indexes[i]];
		const glm::vec3& v1 = vertices[indexes[i + 1]];
		const glm::vec3& v2 = vertices[indexes[i + 2]];
		glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
		normals[indexes[i]] += normal;
		normals[indexes[i + 1]] += normal;
		normals[indexes[i + 2]] += normal;
	}

	for (size_t i = 0; i < normals.size(); ++i) {
		normals[i] = glm::normalize(normals[i]);
	}
	return normals;
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
