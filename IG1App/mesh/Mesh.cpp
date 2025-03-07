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

Mesh::Mesh(Mesh &&m) noexcept {
	mVAO = m.mVAO;
	mVBO = m.mVBO;
	mCBO = m.mCBO;
	mPrimitive = m.mPrimitive;
	mNumVertices = m.mNumVertices;
	vVertices = std::move(m.vVertices);
	vColors = std::move(m.vColors);

	m.mVAO = NONE;
	m.mVBO = NONE;
	m.mCBO = NONE;
	m.mPrimitive = GL_TRIANGLES;
	m.mNumVertices = 0;
	m.vVertices.clear();
	m.vColors.clear();
}

Mesh &Mesh::operator=(Mesh &&m) noexcept {
	if (this != &m) {
		unload();

		mVAO = m.mVAO;
		mVBO = m.mVBO;
		mCBO = m.mCBO;
		mPrimitive = m.mPrimitive;
		mNumVertices = m.mNumVertices;
		vVertices = std::move(m.vVertices);
		vColors = std::move(m.vColors);

		m.mVAO = NONE;
		m.mVBO = NONE;
		m.mCBO = NONE;
		m.mPrimitive = GL_TRIANGLES;
		m.mNumVertices = 0;
		m.vVertices.clear();
		m.vColors.clear();
	}
	return *this;
}

void Mesh::render() const
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
		glm::vec3(-w / 2, -h / 2, 0.0f),
		glm::vec3(w / 2, h / 2, 0.0f),
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

	constexpr static float const start_angle = glm::half_pi<float>();
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

	constexpr static float const start_angle = glm::half_pi<float>();
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
	Mesh* mesh = new Mesh;
	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 14;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));

	return mesh;
}

Mesh*
Mesh::GenerateRGBCube(GLdouble l) {
	Mesh* mesh = new Mesh;

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->mNumVertices = 36;
	mesh->vVertices.reserve(mesh->mNumVertices);

	//Triangulo 1
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));

	//Triangulo 2
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, l / 2));

	//Triangulo 3
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, l / 2));

	//Triangulo 4
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));

	//Triangulo 5
	mesh->vVertices.push_back(vec3(l / 2, l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));

	//Triangulo 6
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));

	//Triangulo 7
	mesh->vVertices.push_back(vec3(l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));

	//Triangulo 8
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, -l / 2));

	//Triangulo 9
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));

	//Triangulo 10
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, l / 2, l / 2));

	//Triangulo 11
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));

	//Triangulo 12
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(l / 2, -l / 2, -l / 2));
	mesh->vVertices.push_back(vec3(-l / 2, -l / 2, l / 2));

	mesh->vColors.reserve(mesh->mNumVertices);

	//Triangulos 1 y 2
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));

	//Triangulos 3 y 4
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));

	//Triangulos 5 y 6
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));

	//Triangulos 7 y 8
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));
	mesh->vColors.emplace_back(vec4(1, 0, 0, 1));

	//Triangulos 9 y 10
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));
	mesh->vColors.emplace_back(vec4(0, 1, 0, 1));

	//Triangulos 11 y 12
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));
	mesh->vColors.emplace_back(vec4(0, 0, 1, 1));

	return mesh;
}

mesh_uv mesh_uv::generate_skibidi_cube(GLfloat side_legth) {
	constexpr static const std::array vertices{
		glm::vec3{0.500000, -0.500000, -0.500000},
		glm::vec3{0.500000, 0.500000, -0.500000},
		glm::vec3{0.500000, -0.500000, 0.500000},
		glm::vec3{0.500000, 0.500000, 0.500000},
		glm::vec3{-0.500000, -0.500000, -0.500000},
		glm::vec3{-0.500000, 0.500000, -0.500000},
		glm::vec3{-0.500000, -0.500000, 0.500000},
		glm::vec3{-0.500000, 0.500000, 0.500000},
		glm::vec3{-0.250000, -0.500000, 0.500000},
		glm::vec3{0.000000, -0.500000, 0.500000},
		glm::vec3{0.250000, -0.500000, 0.500000},
		glm::vec3{0.250000, 0.500000, 0.500000},
		glm::vec3{0.000000, 0.500000, 0.500000},
		glm::vec3{-0.250000, 0.500000, 0.500000},
		glm::vec3{0.250000, -0.500000, -0.500000},
		glm::vec3{-0.000000, -0.500000, -0.500000},
		glm::vec3{-0.250000, -0.500000, -0.500000},
		glm::vec3{-0.250000, 0.500000, -0.500000},
		glm::vec3{-0.000000, 0.500000, -0.500000},
		glm::vec3{0.250000, 0.500000, -0.500000},
		glm::vec3{0.500000, -0.500000, 0.250000},
		glm::vec3{0.500000, -0.500000, -0.000000},
		glm::vec3{0.500000, -0.500000, -0.250000},
		glm::vec3{0.500000, 0.500000, -0.250000},
		glm::vec3{0.500000, 0.500000, -0.000000},
		glm::vec3{0.500000, 0.500000, 0.250000},
		glm::vec3{-0.500000, -0.500000, -0.250000},
		glm::vec3{-0.500000, -0.500000, 0.000000},
		glm::vec3{-0.500000, -0.500000, 0.250000},
		glm::vec3{-0.500000, 0.500000, 0.000000},
		glm::vec3{-0.500000, 0.500000, 0.000000},
		glm::vec3{-0.500000, 0.500000, -0.250000},
		glm::vec3{-0.250000, 0.500000, 0.250000},
		glm::vec3{-0.250000, 0.500000, -0.500000},
		glm::vec3{-0.500000, 0.500000, -0.500000},
		glm::vec3{0.000000, 0.500000, 0.250000},
		glm::vec3{0.250000, -0.500000, 0.250000},
		glm::vec3{-0.000000, 0.500000, -0.250000},
		glm::vec3{0.250000, 0.500000, 0.250000},
		glm::vec3{0.250000, 0.500000, -0.000000},
		glm::vec3{0.250000, 0.500000, -0.250000},
		glm::vec3{0.250000, -0.500000, 0.250000},
		glm::vec3{0.250000, -0.500000, -0.000000},
		glm::vec3{0.250000, -0.500000, -0.250000},
		glm::vec3{0.000000, -0.500000, 0.250000},
		glm::vec3{-0.250000, 0.500000, 0.250000},
		glm::vec3{-0.000000, -0.500000, -0.250000},
		glm::vec3{-0.250000, -0.500000, 0.250000},
		glm::vec3{-0.250000, -0.500000, 0.000000},
		glm::vec3{-0.250000, -0.500000, -0.250000},
		glm::vec3{-0.500000, 0.500000, 0.250000},
		glm::vec3{-0.500000, 0.500000, -0.250000},
		glm::vec3{-0.500000, 0.500000, 0.500000},
		glm::vec3{-0.250000, 0.500000, 0.500000},
		glm::vec3{-0.250000, 0.500000, 0.000000},
		glm::vec3{-0.250000, 0.500000, -0.250000},
		glm::vec3{-0.250000, 0.500000, 0.250000},
		glm::vec3{-0.250000, 0.500000, 0.000000},
		glm::vec3{-0.250000, 0.500000, -0.250000},
		glm::vec3{0.000000, 0.500000, 0.250000},
		glm::vec3{0.000000, 0.500000, 0.000000},
		glm::vec3{-0.000000, 0.500000, -0.250000},
		glm::vec3{0.250000, 0.500000, 0.250000},
		glm::vec3{0.250000, 0.500000, -0.000000},
		glm::vec3{0.250000, 0.500000, -0.250000},
		glm::vec3{0.250000, -0.500000, -0.000000},
		glm::vec3{0.250000, -0.500000, -0.250000},
		glm::vec3{0.000000, -0.500000, 0.250000},
		glm::vec3{0.000000, -0.500000, 0.000000},
		glm::vec3{-0.000000, -0.500000, -0.250000},
		glm::vec3{-0.250000, -0.500000, 0.250000},
		glm::vec3{-0.250000, -0.500000, 0.000000},
		glm::vec3{-0.250000, -0.500000, -0.250000},
		glm::vec3{-0.250000, 0.500000, 0.000000},
		glm::vec3{-0.250000, 0.500000, -0.250000},
		glm::vec3{-0.500000, 0.500000, 0.250000},
	};

	constexpr static const std::array texture_coordinates{
		glm::vec2{0.675347, 0.912625},
		glm::vec2{0.580556, 0.369959},
		glm::vec2{0.675347, 0.120625},
		glm::vec2{0.603303, 0.883292},
		glm::vec2{0.575526, 0.149959},
		glm::vec2{0.603303, 0.149959},
		glm::vec2{0.652778, 0.883292},
		glm::vec2{0.680556, 0.149959},
		glm::vec2{0.652778, 0.149959},
		glm::vec2{0.580734, 0.956625},
		glm::vec2{0.675526, 0.369959},
		glm::vec2{0.580734, 0.076625},
		glm::vec2{0.582658, 0.136483},
		glm::vec2{0.615991, 0.169816},
		glm::vec2{0.615991, 0.136483},
		glm::vec2{0.662171, 0.820858},
		glm::vec2{0.695505, 0.854191},
		glm::vec2{0.662171, 0.854191},
		glm::vec2{0.626858, 0.961999},
		glm::vec2{0.123386, 0.906443},
		glm::vec2{0.626858, 0.906443},
		glm::vec2{0.563213, 0.948983},
		glm::vec2{0.529880, 0.982316},
		glm::vec2{0.529880, 0.948983},
		glm::vec2{0.596547, 0.982316},
		glm::vec2{0.563213, 0.982316},
		glm::vec2{0.603491, 0.117733},
		glm::vec2{0.583491, 0.340858},
		glm::vec2{0.603491, 0.151066},
		glm::vec2{0.649324, 0.136483},
		glm::vec2{0.682658, 0.169816},
		glm::vec2{0.682658, 0.136483},
		glm::vec2{0.649324, 0.169816},
		glm::vec2{0.575526, 0.883292},
		glm::vec2{0.631081, 0.149959},
		glm::vec2{0.631081, 0.883292},
		glm::vec2{0.658859, 0.149959},
		glm::vec2{0.675526, 0.663292},
		glm::vec2{0.580734, 0.090334},
		glm::vec2{0.658859, 0.883292},
		glm::vec2{0.658859, 0.149959},
		glm::vec2{0.615991, 0.036483},
		glm::vec2{0.649324, 0.069816},
		glm::vec2{0.649324, 0.036483},
		glm::vec2{0.651051, 0.107766},
		glm::vec2{0.873273, 0.052211},
		glm::vec2{0.651051, 0.052211},
		glm::vec2{0.651051, 0.064373},
		glm::vec2{0.873273, 0.119929},
		glm::vec2{0.651051, 0.119929},
		glm::vec2{0.682658, 0.069816},
		glm::vec2{0.682658, 0.036483},
		glm::vec2{0.651051, 0.119929},
		glm::vec2{0.873273, 0.064373},
		glm::vec2{0.873273, 0.163322},
		glm::vec2{0.651051, 0.163322},
		glm::vec2{0.703491, 0.117733},
		glm::vec2{0.670158, 0.151066},
		glm::vec2{0.690158, 0.340858},
		glm::vec2{0.715991, 0.103149},
		glm::vec2{0.715991, 0.069816},
		glm::vec2{0.682658, 0.103149},
		glm::vec2{0.715991, 0.136483},
		glm::vec2{0.596547, 0.848983},
		glm::vec2{0.563213, 0.882316},
		glm::vec2{0.563213, 0.848983},
		glm::vec2{0.636865, 0.873208},
		glm::vec2{0.859087, 0.939875},
		glm::vec2{0.859087, 0.873208},
		glm::vec2{0.637328, 0.807672},
		glm::vec2{0.859550, 0.874339},
		glm::vec2{0.859550, 0.807672},
		glm::vec2{0.529880, 0.848983},
		glm::vec2{0.529880, 0.882316},
		glm::vec2{0.636865, 0.940811},
		glm::vec2{0.859087, 0.874144},
		glm::vec2{0.859087, 0.940811},
		glm::vec2{0.635304, 0.939327},
		glm::vec2{0.857526, 0.872660},
		glm::vec2{0.857526, 0.939327},
		glm::vec2{0.626858, 0.904585},
		glm::vec2{0.397691, 0.849030},
		glm::vec2{0.626858, 0.849030},
		glm::vec2{0.595486, 0.875132},
		glm::vec2{0.366319, 0.930688},
		glm::vec2{0.595486, 0.930688},
		glm::vec2{0.607658, 0.856903},
		glm::vec2{0.357658, 0.968014},
		glm::vec2{0.607658, 0.968014},
		glm::vec2{0.575505, 0.662316},
		glm::vec2{0.595505, 0.854191},
		glm::vec2{0.562172, 0.854191},
		glm::vec2{0.629880, 0.882316},
		glm::vec2{0.596547, 0.915649},
		glm::vec2{0.596547, 0.882316},
		glm::vec2{0.629880, 0.948983},
		glm::vec2{0.596547, 0.948983},
		glm::vec2{0.582658, 0.036483},
		glm::vec2{0.615991, 0.069816},
		glm::vec2{0.582658, 0.069816},
		glm::vec2{0.615991, 0.103149},
		glm::vec2{0.582658, 0.103149},
		glm::vec2{0.569444, 0.883292},
		glm::vec2{0.597222, 0.149959},
		glm::vec2{0.569444, 0.149959},
		glm::vec2{0.597222, 0.883292},
		glm::vec2{0.625000, 0.149959},
		glm::vec2{0.625000, 0.883292},
		glm::vec2{0.669444, 0.663292},
		glm::vec2{0.590278, 0.076625},
		glm::vec2{0.669444, 0.369959},
		glm::vec2{0.641667, 0.961070},
		glm::vec2{0.108333, 0.905514},
		glm::vec2{0.641667, 0.905514},
		glm::vec2{0.108333, 0.849959},
		glm::vec2{0.641667, 0.849959},
		glm::vec2{0.123283, 0.888740},
		glm::vec2{0.067727, 0.833184},
		glm::vec2{0.123283, 0.833184},
		glm::vec2{0.178838, 0.888740},
		glm::vec2{0.178838, 0.833184},
		glm::vec2{0.342553, 0.888740},
		glm::vec2{0.159220, 0.833184},
		glm::vec2{0.342553, 0.833184},
		glm::vec2{0.164602, 0.778497},
		glm::vec2{0.342380, 0.834052},
		glm::vec2{0.164602, 0.834052},
		glm::vec2{0.954880, 0.860094},
		glm::vec2{0.899324, 0.915649},
		glm::vec2{0.899324, 0.860094},
		glm::vec2{0.954880, 0.971205},
		glm::vec2{0.899324, 0.971205},
		glm::vec2{0.843769, 0.860094},
		glm::vec2{0.843769, 0.915649},
		glm::vec2{0.843769, 0.971205},
		glm::vec2{0.635304, 0.872660},
		glm::vec2{0.857526, 0.805993},
		glm::vec2{0.636865, 0.874144},
		glm::vec2{0.859087, 0.807478},
		glm::vec2{0.636865, 0.806541},
		glm::vec2{0.859087, 0.806541},
		glm::vec2{0.637328, 0.874339},
		glm::vec2{0.859550, 0.941005},
		glm::vec2{0.832658, 0.036483},
		glm::vec2{0.899324, 0.103149},
		glm::vec2{0.899324, 0.036483},
		glm::vec2{0.832658, 0.169816},
		glm::vec2{0.899324, 0.169816},
		glm::vec2{0.965991, 0.036483},
		glm::vec2{0.965991, 0.103149},
		glm::vec2{0.965991, 0.169816},
		glm::vec2{0.651051, 0.175484},
		glm::vec2{0.873273, 0.119929},
		glm::vec2{0.873273, 0.175484},
		glm::vec2{0.873273, 0.107766},
		glm::vec2{0.607658, 0.968014},
		glm::vec2{0.357658, 0.856903},
		glm::vec2{0.595486, 0.817841},
		glm::vec2{0.123264, 0.873396},
		glm::vec2{0.595486, 0.873396},
		glm::vec2{0.641667, 0.818709},
		glm::vec2{0.108333, 0.874264},
		glm::vec2{0.641667, 0.874264},
		glm::vec2{0.108333, 0.929820},
		glm::vec2{0.641667, 0.929820},
		glm::vec2{0.580556, 0.663292},
		glm::vec2{0.680556, 0.883292},
		glm::vec2{0.582658, 0.169816},
		glm::vec2{0.682172, 0.662316},
		glm::vec2{0.397691, 0.961999},
		glm::vec2{0.570158, 0.117733},
		glm::vec2{0.658859, 0.883292},
		glm::vec2{0.580734, 0.911667},
		glm::vec2{0.670158, 0.117733},
		glm::vec2{0.636865, 0.939875},
		glm::vec2{0.123386, 0.904585},
		glm::vec2{0.123264, 0.875132},
		glm::vec2{0.595505, 0.820858},
		glm::vec2{0.629880, 0.915649},
		glm::vec2{0.590278, 0.956625},
		glm::vec2{0.108333, 0.961070},
		glm::vec2{0.067727, 0.888740},
		glm::vec2{0.159220, 0.888740},
		glm::vec2{0.342380, 0.778497},
		glm::vec2{0.954880, 0.915649},
		glm::vec2{0.635304, 0.805993},
		glm::vec2{0.636865, 0.807478},
		glm::vec2{0.637328, 0.941005},
		glm::vec2{0.832658, 0.103149},
		glm::vec2{0.366319, 0.817841},
		glm::vec2{0.108333, 0.818709},
	};

	using vec2_u16 = glm::vec<2, uint16_t>;
	constexpr static const std::array indices{
		vec2_u16{26, 1}, vec2_u16{3, 2}, vec2_u16{21, 3},
		vec2_u16{14, 4}, vec2_u16{7, 5}, vec2_u16{9, 6},
		vec2_u16{32, 7}, vec2_u16{5, 8}, vec2_u16{27, 9},
		vec2_u16{20, 10}, vec2_u16{1, 11}, vec2_u16{15, 12},
		vec2_u16{27, 13}, vec2_u16{17, 14}, vec2_u16{50, 15},
		vec2_u16{24, 16}, vec2_u16{20, 17}, vec2_u16{41, 18},
		vec2_u16{18, 19}, vec2_u16{75, 20}, vec2_u16{56, 21},
		vec2_u16{38, 22}, vec2_u16{18, 23}, vec2_u16{56, 24},
		vec2_u16{38, 22}, vec2_u16{20, 25}, vec2_u16{19, 26},
		vec2_u16{44, 27}, vec2_u16{1, 28}, vec2_u16{23, 29},
		vec2_u16{47, 30}, vec2_u16{15, 31}, vec2_u16{44, 32},
		vec2_u16{50, 15}, vec2_u16{16, 33}, vec2_u16{47, 30},
		vec2_u16{6, 34}, vec2_u16{17, 6}, vec2_u16{5, 5},
		vec2_u16{18, 4}, vec2_u16{16, 35}, vec2_u16{17, 6},
		vec2_u16{19, 36}, vec2_u16{15, 37}, vec2_u16{16, 35},
		vec2_u16{4, 38}, vec2_u16{11, 39}, vec2_u16{3, 11},
		vec2_u16{12, 40}, vec2_u16{10, 35}, vec2_u16{11, 41},
		vec2_u16{13, 36}, vec2_u16{9, 6}, vec2_u16{10, 35},
		vec2_u16{9, 42}, vec2_u16{45, 43}, vec2_u16{10, 44},
		vec2_u16{49, 45}, vec2_u16{71, 46}, vec2_u16{48, 47},
		vec2_u16{48, 48}, vec2_u16{68, 49}, vec2_u16{45, 50},
		vec2_u16{10, 44}, vec2_u16{42, 51}, vec2_u16{11, 52},
		vec2_u16{47, 53}, vec2_u16{73, 54}, vec2_u16{50, 48},
		vec2_u16{43, 45}, vec2_u16{67, 55}, vec2_u16{44, 56},
		vec2_u16{11, 57}, vec2_u16{21, 58}, vec2_u16{3, 59},
		vec2_u16{42, 51}, vec2_u16{22, 60}, vec2_u16{21, 61},
		vec2_u16{43, 62}, vec2_u16{23, 63}, vec2_u16{22, 60},
		vec2_u16{12, 64}, vec2_u16{36, 65}, vec2_u16{13, 66},
		vec2_u16{36, 67}, vec2_u16{63, 68}, vec2_u16{60, 69},
		vec2_u16{39, 70}, vec2_u16{64, 71}, vec2_u16{63, 72},
		vec2_u16{14, 73}, vec2_u16{36, 65}, vec2_u16{33, 74},
		vec2_u16{41, 75}, vec2_u16{62, 76}, vec2_u16{65, 77},
		vec2_u16{56, 78}, vec2_u16{58, 79}, vec2_u16{59, 80},
		vec2_u16{33, 81}, vec2_u16{54, 82}, vec2_u16{14, 83},
		vec2_u16{32, 84}, vec2_u16{35, 85}, vec2_u16{6, 86},
		vec2_u16{6, 87}, vec2_u16{34, 88}, vec2_u16{18, 89},
		vec2_u16{4, 90}, vec2_u16{39, 91}, vec2_u16{12, 92},
		vec2_u16{26, 93}, vec2_u16{40, 94}, vec2_u16{39, 95},
		vec2_u16{40, 94}, vec2_u16{24, 96}, vec2_u16{41, 97},
		vec2_u16{7, 98}, vec2_u16{48, 99}, vec2_u16{9, 42},
		vec2_u16{29, 100}, vec2_u16{49, 101}, vec2_u16{48, 99},
		vec2_u16{28, 102}, vec2_u16{50, 15}, vec2_u16{49, 101},
		vec2_u16{8, 103}, vec2_u16{29, 104}, vec2_u16{7, 105},
		vec2_u16{51, 106}, vec2_u16{28, 107}, vec2_u16{29, 104},
		vec2_u16{31, 108}, vec2_u16{27, 9}, vec2_u16{28, 107},
		vec2_u16{2, 109}, vec2_u16{23, 110}, vec2_u16{1, 111},
		vec2_u16{24, 7}, vec2_u16{22, 107}, vec2_u16{23, 9},
		vec2_u16{25, 108}, vec2_u16{21, 104}, vec2_u16{22, 107},
		vec2_u16{56, 112}, vec2_u16{74, 113}, vec2_u16{55, 114},
		vec2_u16{55, 114}, vec2_u16{46, 115}, vec2_u16{33, 116},
		vec2_u16{74, 117}, vec2_u16{52, 118}, vec2_u16{30, 119},
		vec2_u16{46, 120}, vec2_u16{30, 119}, vec2_u16{76, 121},
		vec2_u16{54, 122}, vec2_u16{76, 123}, vec2_u16{53, 124},
		vec2_u16{75, 125}, vec2_u16{35, 126}, vec2_u16{52, 127},
		vec2_u16{63, 128}, vec2_u16{61, 129}, vec2_u16{60, 130},
		vec2_u16{61, 129}, vec2_u16{65, 131}, vec2_u16{62, 132},
		vec2_u16{57, 133}, vec2_u16{61, 129}, vec2_u16{58, 134},
		vec2_u16{61, 129}, vec2_u16{59, 135}, vec2_u16{58, 134},
		vec2_u16{55, 136}, vec2_u16{57, 137}, vec2_u16{58, 79},
		vec2_u16{38, 138}, vec2_u16{59, 139}, vec2_u16{62, 76},
		vec2_u16{33, 140}, vec2_u16{60, 69}, vec2_u16{57, 141},
		vec2_u16{40, 142}, vec2_u16{65, 143}, vec2_u16{64, 71},
		vec2_u16{71, 144}, vec2_u16{69, 145}, vec2_u16{68, 146},
		vec2_u16{69, 145}, vec2_u16{73, 147}, vec2_u16{70, 148},
		vec2_u16{37, 149}, vec2_u16{69, 145}, vec2_u16{66, 150},
		vec2_u16{69, 145}, vec2_u16{67, 151}, vec2_u16{66, 150},
		vec2_u16{44, 152}, vec2_u16{70, 153}, vec2_u16{47, 53},
		vec2_u16{45, 50}, vec2_u16{37, 154}, vec2_u16{42, 152},
		vec2_u16{42, 47}, vec2_u16{66, 155}, vec2_u16{43, 45},
		vec2_u16{50, 56}, vec2_u16{72, 155}, vec2_u16{49, 45},
		vec2_u16{14, 156}, vec2_u16{53, 157}, vec2_u16{8, 87},
		vec2_u16{8, 158}, vec2_u16{76, 159}, vec2_u16{51, 160},
		vec2_u16{51, 161}, vec2_u16{30, 162}, vec2_u16{31, 163},
		vec2_u16{31, 163}, vec2_u16{52, 164}, vec2_u16{32, 165},
		vec2_u16{26, 1}, vec2_u16{4, 166}, vec2_u16{3, 2},
		vec2_u16{14, 4}, vec2_u16{8, 34}, vec2_u16{7, 5},
		vec2_u16{32, 7}, vec2_u16{6, 167}, vec2_u16{5, 8},
		vec2_u16{20, 10}, vec2_u16{2, 38}, vec2_u16{1, 11},
		vec2_u16{27, 13}, vec2_u16{5, 168}, vec2_u16{17, 14},
		vec2_u16{24, 16}, vec2_u16{2, 169}, vec2_u16{20, 17},
		vec2_u16{18, 19}, vec2_u16{34, 170}, vec2_u16{75, 20},
		vec2_u16{38, 22}, vec2_u16{19, 26}, vec2_u16{18, 23},
		vec2_u16{38, 22}, vec2_u16{41, 97}, vec2_u16{20, 25},
		vec2_u16{44, 27}, vec2_u16{15, 171}, vec2_u16{1, 28},
		vec2_u16{47, 30}, vec2_u16{16, 33}, vec2_u16{15, 31},
		vec2_u16{50, 15}, vec2_u16{17, 14}, vec2_u16{16, 33},
		vec2_u16{6, 34}, vec2_u16{18, 4}, vec2_u16{17, 6},
		vec2_u16{18, 4}, vec2_u16{19, 36}, vec2_u16{16, 35},
		vec2_u16{19, 36}, vec2_u16{20, 172}, vec2_u16{15, 37},
		vec2_u16{4, 38}, vec2_u16{12, 173}, vec2_u16{11, 39},
		vec2_u16{12, 40}, vec2_u16{13, 36}, vec2_u16{10, 35},
		vec2_u16{13, 36}, vec2_u16{14, 4}, vec2_u16{9, 6},
		vec2_u16{9, 42}, vec2_u16{48, 99}, vec2_u16{45, 43},
		vec2_u16{49, 45}, vec2_u16{72, 155}, vec2_u16{71, 46},
		vec2_u16{48, 48}, vec2_u16{71, 54}, vec2_u16{68, 49},
		vec2_u16{10, 44}, vec2_u16{45, 43}, vec2_u16{42, 51},
		vec2_u16{47, 53}, vec2_u16{70, 153}, vec2_u16{73, 54},
		vec2_u16{43, 45}, vec2_u16{66, 155}, vec2_u16{67, 55},
		vec2_u16{11, 57}, vec2_u16{42, 174}, vec2_u16{21, 58},
		vec2_u16{42, 51}, vec2_u16{43, 62}, vec2_u16{22, 60},
		vec2_u16{43, 62}, vec2_u16{44, 32}, vec2_u16{23, 63},
		vec2_u16{12, 64}, vec2_u16{39, 95}, vec2_u16{36, 65},
		vec2_u16{36, 67}, vec2_u16{39, 175}, vec2_u16{63, 68},
		vec2_u16{39, 70}, vec2_u16{40, 142}, vec2_u16{64, 71},
		vec2_u16{14, 73}, vec2_u16{13, 66}, vec2_u16{36, 65},
		vec2_u16{41, 75}, vec2_u16{38, 138}, vec2_u16{62, 76},
		vec2_u16{56, 78}, vec2_u16{55, 136}, vec2_u16{58, 79},
		vec2_u16{33, 81}, vec2_u16{46, 176}, vec2_u16{54, 82},
		vec2_u16{32, 84}, vec2_u16{52, 177}, vec2_u16{35, 85},
		vec2_u16{6, 87}, vec2_u16{35, 157}, vec2_u16{34, 88},
		vec2_u16{4, 90}, vec2_u16{26, 178}, vec2_u16{39, 91},
		vec2_u16{26, 93}, vec2_u16{25, 179}, vec2_u16{40, 94},
		vec2_u16{40, 94}, vec2_u16{25, 179}, vec2_u16{24, 96},
		vec2_u16{7, 98}, vec2_u16{29, 100}, vec2_u16{48, 99},
		vec2_u16{29, 100}, vec2_u16{28, 102}, vec2_u16{49, 101},
		vec2_u16{28, 102}, vec2_u16{27, 13}, vec2_u16{50, 15},
		vec2_u16{8, 103}, vec2_u16{51, 106}, vec2_u16{29, 104},
		vec2_u16{51, 106}, vec2_u16{31, 108}, vec2_u16{28, 107},
		vec2_u16{31, 108}, vec2_u16{32, 7}, vec2_u16{27, 9},
		vec2_u16{2, 109}, vec2_u16{24, 180}, vec2_u16{23, 110},
		vec2_u16{24, 7}, vec2_u16{25, 108}, vec2_u16{22, 107},
		vec2_u16{25, 108}, vec2_u16{26, 106}, vec2_u16{21, 104},
		vec2_u16{56, 112}, vec2_u16{75, 181}, vec2_u16{74, 113},
		vec2_u16{55, 114}, vec2_u16{74, 113}, vec2_u16{46, 115},
		vec2_u16{74, 117}, vec2_u16{75, 182}, vec2_u16{52, 118},
		vec2_u16{46, 120}, vec2_u16{74, 117}, vec2_u16{30, 119},
		vec2_u16{54, 122}, vec2_u16{46, 183}, vec2_u16{76, 123},
		vec2_u16{75, 125}, vec2_u16{34, 184}, vec2_u16{35, 126},
		vec2_u16{63, 128}, vec2_u16{64, 185}, vec2_u16{61, 129},
		vec2_u16{61, 129}, vec2_u16{64, 185}, vec2_u16{65, 131},
		vec2_u16{57, 133}, vec2_u16{60, 130}, vec2_u16{61, 129},
		vec2_u16{61, 129}, vec2_u16{62, 132}, vec2_u16{59, 135},
		vec2_u16{55, 136}, vec2_u16{33, 186}, vec2_u16{57, 137},
		vec2_u16{38, 138}, vec2_u16{56, 187}, vec2_u16{59, 139},
		vec2_u16{33, 140}, vec2_u16{36, 67}, vec2_u16{60, 69},
		vec2_u16{40, 142}, vec2_u16{41, 188}, vec2_u16{65, 143},
		vec2_u16{71, 144}, vec2_u16{72, 189}, vec2_u16{69, 145},
		vec2_u16{69, 145}, vec2_u16{72, 189}, vec2_u16{73, 147},
		vec2_u16{37, 149}, vec2_u16{68, 146}, vec2_u16{69, 145},
		vec2_u16{69, 145}, vec2_u16{70, 148}, vec2_u16{67, 151},
		vec2_u16{44, 152}, vec2_u16{67, 154}, vec2_u16{70, 153},
		vec2_u16{45, 50}, vec2_u16{68, 49}, vec2_u16{37, 154},
		vec2_u16{42, 47}, vec2_u16{37, 46}, vec2_u16{66, 155},
		vec2_u16{50, 56}, vec2_u16{73, 55}, vec2_u16{72, 155},
		vec2_u16{14, 156}, vec2_u16{54, 88}, vec2_u16{53, 157},
		vec2_u16{8, 158}, vec2_u16{53, 190}, vec2_u16{76, 159},
		vec2_u16{51, 161}, vec2_u16{76, 191}, vec2_u16{30, 162},
		vec2_u16{31, 163}, vec2_u16{30, 162}, vec2_u16{52, 164},
	};

	std::vector<glm::vec3> verts{indices.size()};
	std::vector<glm::vec2> uvs{indices.size()};
	std::vector<glm::vec4> color_fill{indices.size()};

	for (size_t i = 0; i < indices.size() / 3; ++i) {
		const size_t index = i * 3;
		const auto a_idx = indices[index] - vec2_u16{1};
		const auto b_idx = indices[index + 1] - vec2_u16{1};
		const auto c_idx = indices[index + 2] - vec2_u16{1};

		const glm::vec3 a = vertices[a_idx.x];
		const glm::vec3 b = vertices[b_idx.x];
		const glm::vec3 c = vertices[c_idx.x];

		const glm::vec2 a_uv = texture_coordinates[a_idx.y];
		const glm::vec2 b_uv = texture_coordinates[b_idx.y];
		const glm::vec2 c_uv = texture_coordinates[c_idx.y];

		verts.at(index) 	 = (a * side_legth);
		verts.at(index + 1) = (b * side_legth);
		verts.at(index + 2) = (c * side_legth);
		
		uvs.at(index) 	   = (a_uv);
		uvs.at(index + 1) = (b_uv);
		uvs.at(index + 2) = (c_uv);
		
		constexpr static const glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		color_fill.at(index) 	  = (color);
		color_fill.at(index + 1) = (color);
		color_fill.at(index + 2) = (color);
	}

	mesh_uv mesh{};
	mesh.vVertices = std::move(verts);
	mesh.vertex_uv2_f32 = std::move(uvs);
	mesh.vColors = std::move(color_fill);
	mesh.mPrimitive = GL_TRIANGLES;
	mesh.mNumVertices = mesh.vVertices.size();
	
    return std::move(mesh);
}

mesh_uv mesh_uv::generate_rectangle_uv(const GLfloat w, const GLfloat h, const glm::vec2 uv1, const glm::vec2 uv2, std::array<glm::vec4, 4> colors) {
	const GLfloat half_w = w * 0.5f;
	const GLfloat half_h = h * 0.5f;
	
	std::vector<glm::vec3> verts{
		{-half_w, 0.0f, half_h},
		{-half_w, 0.0f, -half_h},
		{half_w, 0.0f, half_h},
		{half_w, 0.0f, -half_h}
	};
	std::vector<glm::vec2> uvs{
		{uv1.x, uv1.y},
		{uv1.x, uv2.y},
		{uv2.x, uv1.y},
		{uv2.x, uv2.y}
	};
	std::vector<glm::vec4> color_fill{
		colors[0],
		colors[1],
		colors[2],
		colors[3]
	};

	mesh_uv mesh{};
	mesh.vVertices = std::move(verts);
	mesh.vertex_uv2_f32 = std::move(uvs);
	mesh.vColors = std::move(color_fill);
	mesh.mPrimitive = GL_TRIANGLE_STRIP;
	mesh.mNumVertices = mesh.vVertices.size();

	return mesh;
}
mesh_uv mesh_uv::generate_box_hull(const GLfloat side_length, const glm::vec2 uv1, const glm::vec2 uv2, std::array<glm::vec4, 4> colors) {
    const GLfloat half_side = side_length * 0.5f;

	std::vector<glm::vec3> verts{
		{-half_side, half_side, -half_side},
		{-half_side, -half_side, -half_side},
		{half_side, half_side, -half_side},
		{half_side, -half_side, -half_side},

		{half_side, half_side, half_side},
		{half_side, -half_side, half_side},
		{-half_side, half_side, half_side},
		{-half_side, -half_side, half_side},

		{-half_side, half_side, -half_side},
		{-half_side, -half_side, -half_side},
	};
	std::vector<glm::vec2> uvs{
		{uv1.x, uv1.y},
		{uv1.x, uv2.y},
		{uv2.x, uv1.y},
		{uv2.x, uv2.y},

		{uv1.x, uv1.y},
		{uv1.x, uv2.y},
		{uv2.x, uv1.y},
		{uv2.x, uv2.y},

		{uv1.x, uv1.y},
		{uv1.x, uv2.y},
	};
	std::vector<glm::vec4> color_fill{
		colors[0],
		colors[1],
		colors[2],
		colors[3],

		colors[0],
		colors[1],
		colors[2],
		colors[3],
		
		colors[0],
		colors[1],
	};

	mesh_uv mesh{};
	mesh.vVertices = std::move(verts);
	mesh.vertex_uv2_f32 = std::move(uvs);
	mesh.vColors = std::move(color_fill);
	mesh.mPrimitive = GL_TRIANGLE_STRIP;
	mesh.mNumVertices = mesh.vVertices.size();

	return mesh;
};

mesh_uv mesh_uv::generate_stellated_pyramid(
	const GLfloat height,
	const GLfloat outter_radius,
	const GLfloat inner_radius,
	const GLuint base_vertex_count
) {
	const float delta_angle = glm::two_pi<float>() / (base_vertex_count * 2.0f);

	std::vector<glm::vec3> verts(base_vertex_count * 2);
	std::vector<glm::vec2> uvs(base_vertex_count * 2);
	std::vector<glm::vec4> color_fill(base_vertex_count * 2);
	
	const size_t rect_count = base_vertex_count >> 1;
	for (size_t i = 0; i < rect_count; i++) {
		const float angles[3] = {
			delta_angle * (i + 1),
			delta_angle * i,
			delta_angle * (i + 2),
		};
		const std::array<glm::vec3, 4> vertices{
			glm::vec3{inner_radius * glm::cos(angles[1]), height, inner_radius * glm::sin(angles[1])},
			glm::vec3{outter_radius * glm::cos(angles[0]), height, outter_radius * glm::sin(angles[0])},
			glm::vec3{0.0f, 0.0f, 0.0f},
			glm::vec3{outter_radius * glm::cos(angles[2]), height, outter_radius * glm::sin(angles[2])},
		};

		for (size_t j = 0; j < 4; j++) {
			const size_t index = i * 4 + j;
			verts.at(index) = vertices[j];
			uvs.at(index) = glm::vec2{0.0f, 0.0f};
			color_fill.at(index) = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
		}
	}

	mesh_uv mesh{};
	mesh.vVertices = std::move(verts);
	mesh.vertex_uv2_f32 = std::move(uvs);
	mesh.vColors = std::move(color_fill);
	mesh.mPrimitive = GL_TRIANGLE_STRIP;
	mesh.mNumVertices = mesh.vVertices.size();

	return mesh;
}

void mesh_uv::load()
{
    Mesh::load();
	assert(vertex_uv2_f32.size() > 0);
	assert(vertex_uv2_f32.size() == vVertices.size());

	glGenBuffers(1, &attribute_buffer_uv2_f32);
	glBindBuffer(GL_ARRAY_BUFFER, attribute_buffer_uv2_f32);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertex_uv2_f32.size(), vertex_uv2_f32.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glEnableVertexAttribArray(2);
}

void mesh_uv::unload() {
	Mesh::unload();
	if (attribute_buffer_uv2_f32 != NONE) {
		glDeleteBuffers(1, &attribute_buffer_uv2_f32);
		attribute_buffer_uv2_f32 = NONE;
	}
}
