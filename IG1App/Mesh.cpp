#include "Mesh.h"
#include <cassert>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load()
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}
	}
}

void
Mesh::unload()
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		mVAO = NONE;
		mVBO = NONE;

		if (mCBO != NONE) {
			glDeleteBuffers(1, &mCBO);
			mCBO = NONE;
		}
	}
}

void
Mesh::render() const
{
	assert(mVAO != NONE);

	//glVertexPointer(3, GL_FLOAT, 0, vVertices.data());
	glBindVertexArray(mVAO);
	//glEnableVertexAttribArray(mVAO);
	draw();
	//glDisableVertexAttribArray(mVAO);
}

Mesh* Mesh::generate_rectangle(GLdouble w, GLdouble h, std::array<glm::vec4, 4> &&vertex_colours)
{
	std::vector<glm::vec3> vertices = {
		glm::vec3(-w / 2, h / 2, 0.0f),
		glm::vec3(w / 2, h / 2, 0.0f),
		glm::vec3(-w / 2, -h / 2, 0.0f),
		glm::vec3(w / 2, -h / 2, 0.0f),
	};

	std::vector<glm::vec4> colors{ std::begin(vertex_colours), std::end(vertex_colours) };

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices = std::move(vertices);
	mesh->vColors = std::move(colors);

	return mesh;
}

Mesh* Mesh::generate_regular_polygon(GLuint num, GLdouble r)
{
	assert(num > 2 && "error: cannot create mesh with less than 3 vertices");
	std::vector<glm::vec3> vertices(num);
	std::vector<glm::vec4> colors(num);

	constexpr float const start_angle = glm::half_pi<float>();
	float const angle_delta = glm::two_pi<float>() / num;
	for (size_t i = 0; i < num; ++i) {
		float const angle = start_angle + i * angle_delta;

		glm::vec3 const position = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * float(r);
		glm::vec4 const color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		vertices[i] = position;
		colors[i] = color;
	}

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = num;
	mesh->vVertices = std::move(vertices);
	mesh->vColors = std::move(colors);

	return mesh;
}

Mesh* Mesh::generate_regular_polygon(GLuint num, GLdouble r, vector<glm::vec4> const& vColors, GLuint primitive = GL_LINE_LOOP)
{
	assert(num > 2 && "error: cannot create mesh with less than 3 vertices");
	assert(vColors.size() == num && "error: vertex colors size doesn't match vertex size");
	std::vector<glm::vec3> vertices(num);
	std::vector<glm::vec4> colors(num);

	constexpr float const start_angle = glm::half_pi<float>();
	float const angle_delta = glm::two_pi<float>() / num;
	for (size_t i = 0; i < num; ++i) {
		float const angle = start_angle + i * angle_delta;

		glm::vec3 const position = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * float(r);

		vertices[i] = position;
		colors[i] = vColors[i];
	}

	Mesh* mesh = new Mesh();
	mesh->mPrimitive = primitive;
	mesh->mNumVertices = num;
	mesh->vVertices = std::move(vertices);
	mesh->vColors = std::move(colors);

	return mesh;
}

Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*Mesh::generateCube(GLdouble l)
{
	std::vector<glm::vec3> vertices = {
		glm::vec3(-l / 2, l / 2, 0.0f),
		glm::vec3(l / 2, l / 2, 0.0f),
		glm::vec3(-l / 2, -l / 2, 0.0f),
		glm::vec3(l / 2, -l / 2, 0.0f),
		glm::vec3(0.0f, l / 2, -l / 2),
	};
	return nullptr;
}