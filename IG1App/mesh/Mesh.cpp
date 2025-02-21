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

mesh_uv mesh_uv::generate_skibidi_cube(GLdouble side_legth) {
	constexpr static const std::array vertices{
		glm::vec3{-0.500000, -0.500000, 0.500000},
		glm::vec3{-0.500000, 0.500000, 0.500000},
		glm::vec3{-0.500000, -0.500000, -0.500000},
		glm::vec3{-0.500000, 0.500000, -0.500000},
		glm::vec3{0.500000, -0.500000, 0.500000},
		glm::vec3{0.500000, 0.500000, 0.500000},
		glm::vec3{0.500000, -0.500000, -0.500000},
		glm::vec3{0.500000, 0.500000, -0.500000},
		glm::vec3{0.250000, -0.500000, -0.500000},
		glm::vec3{0.000000, -0.500000, -0.500000},
		glm::vec3{-0.250000, -0.500000, -0.500000},
		glm::vec3{-0.250000, 0.500000, -0.500000},
		glm::vec3{0.000000, 0.500000, -0.500000},
		glm::vec3{0.250000, 0.500000, -0.500000},
		glm::vec3{-0.250000, -0.500000, 0.500000},
		glm::vec3{0.000000, -0.500000, 0.500000},
		glm::vec3{0.250000, -0.500000, 0.500000},
		glm::vec3{0.250000, 0.500000, 0.500000},
		glm::vec3{0.000000, 0.500000, 0.500000},
		glm::vec3{-0.250000, 0.500000, 0.500000},
		glm::vec3{-0.500000, -0.500000, -0.250000},
		glm::vec3{-0.500000, -0.500000, 0.000000},
		glm::vec3{-0.500000, -0.500000, 0.250000},
		glm::vec3{-0.500000, 0.500000, 0.250000},
		glm::vec3{-0.500000, 0.500000, 0.000000},
		glm::vec3{-0.500000, 0.500000, -0.250000},
		glm::vec3{0.500000, -0.500000, 0.250000},
		glm::vec3{0.500000, -0.500000, 0.000000},
		glm::vec3{0.500000, -0.500000, -0.250000},
		glm::vec3{0.500000, 0.500000, -0.250000},
		glm::vec3{0.500000, 0.500000, 0.000000},
		glm::vec3{0.500000, 0.500000, 0.250000},
		glm::vec3{0.250000, 0.500000, -0.250000},
		glm::vec3{0.250000, 0.500000, 0.000000},
		glm::vec3{0.250000, 0.500000, 0.250000},
		glm::vec3{0.000000, 0.500000, -0.250000},
		glm::vec3{0.000000, 0.500000, 0.000000},
		glm::vec3{0.000000, 0.500000, 0.250000},
		glm::vec3{-0.250000, 0.500000, -0.250000},
		glm::vec3{-0.250000, 0.500000, 0.000000},
		glm::vec3{-0.250000, 0.500000, 0.250000},
		glm::vec3{-0.250000, -0.500000, -0.250000},
		glm::vec3{-0.250000, -0.500000, 0.000000},
		glm::vec3{-0.250000, -0.500000, 0.250000},
		glm::vec3{0.000000, -0.500000, -0.250000},
		glm::vec3{0.000000, -0.500000, 0.000000},
		glm::vec3{0.000000, -0.500000, 0.250000},
		glm::vec3{0.250000, -0.500000, -0.250000},
		glm::vec3{0.250000, -0.500000, 0.000000},
		glm::vec3{0.250000, -0.500000, 0.250000},
	};

	constexpr static const std::array texture_coordinates{
		glm::vec2{0.000000, 0.750000},
		glm::vec2{0.000000, 1.000000},
		glm::vec2{0.750000, 1.000000},
		glm::vec2{1.000000, 1.000000},
		glm::vec2{1.000000, 0.250000},
		glm::vec2{1.000000, 0.000000},
		glm::vec2{0.250000, 0.000000},
		glm::vec2{0.000000, 0.000000},
		glm::vec2{0.875000, 0.000000},
		glm::vec2{0.875000, 0.250000},
		glm::vec2{0.000000, 0.250000},
		glm::vec2{0.125000, 0.000000},
		glm::vec2{0.125000, 0.250000},
		glm::vec2{0.375000, 0.250000},
		glm::vec2{0.500000, 0.000000},
		glm::vec2{0.500000, 0.250000},
		glm::vec2{0.250000, 0.250000},
		glm::vec2{0.375000, 0.000000},
		glm::vec2{0.625000, 0.250000},
		glm::vec2{0.750000, 0.250000},
		glm::vec2{0.625000, 0.000000},
		glm::vec2{0.750000, 0.000000},
		glm::vec2{0.750000, 0.000000},
		glm::vec2{0.250000, 1.000000},
		glm::vec2{0.500000, 1.000000},
		glm::vec2{0.875000, 1.000000},
		glm::vec2{0.750000, 0.750000},
		glm::vec2{0.750000, 1.000000},
		glm::vec2{0.875000, 0.750000},
		glm::vec2{0.750000, 0.500000},
		glm::vec2{0.875000, 0.500000},
		glm::vec2{0.625000, 0.750000},
		glm::vec2{0.625000, 1.000000},
		glm::vec2{0.625000, 0.500000},
		glm::vec2{0.500000, 0.750000},
		glm::vec2{0.500000, 0.500000},
		glm::vec2{0.125000, 1.000000},
		glm::vec2{0.250000, 0.750000},
		glm::vec2{0.125000, 0.750000},
		glm::vec2{0.250000, 0.500000},
		glm::vec2{0.125000, 0.500000},
		glm::vec2{0.375000, 0.750000},
		glm::vec2{0.375000, 1.000000},
		glm::vec2{0.375000, 0.500000},
		glm::vec2{0.000000, 0.500000},
		glm::vec2{1.000000, 0.750000},
		glm::vec2{1.000000, 0.500000},
	};
	using vec2_u16 = glm::vec<2, uint16_t>;
	constexpr static const std::array indices{
		vec2_u16{26, 1}, vec2_u16{3, 2}, vec2_u16{21, 1},
		vec2_u16{14, 3}, vec2_u16{7, 4}, vec2_u16{9, 3},
		vec2_u16{32, 5}, vec2_u16{5, 6}, vec2_u16{27, 5},
		vec2_u16{20, 7}, vec2_u16{1, 8}, vec2_u16{15, 7},
		vec2_u16{27, 5}, vec2_u16{17, 9}, vec2_u16{50, 10},
		vec2_u16{24, 11}, vec2_u16{20, 12}, vec2_u16{41, 13},
		vec2_u16{35, 14}, vec2_u16{6, 15}, vec2_u16{32, 16},
		vec2_u16{38, 17}, vec2_u16{18, 18}, vec2_u16{35, 14},
		vec2_u16{41, 13}, vec2_u16{19, 7}, vec2_u16{38, 17},
		vec2_u16{44, 19}, vec2_u16{1, 15}, vec2_u16{23, 16},
		vec2_u16{47, 20}, vec2_u16{15, 21}, vec2_u16{44, 19},
		vec2_u16{50, 10}, vec2_u16{16, 22}, vec2_u16{47, 20},
		vec2_u16{6, 6}, vec2_u16{17, 23}, vec2_u16{5, 6},
		vec2_u16{18, 23}, vec2_u16{16, 15}, vec2_u16{17, 23},
		vec2_u16{19, 15}, vec2_u16{15, 7}, vec2_u16{16, 15},
		vec2_u16{4, 2}, vec2_u16{11, 24}, vec2_u16{3, 2},
		vec2_u16{12, 24}, vec2_u16{10, 25}, vec2_u16{11, 24},
		vec2_u16{13, 25}, vec2_u16{9, 3}, vec2_u16{10, 25},
		vec2_u16{9, 26}, vec2_u16{45, 27}, vec2_u16{10, 28},
		vec2_u16{48, 29}, vec2_u16{46, 30}, vec2_u16{45, 27},
		vec2_u16{49, 31}, vec2_u16{47, 20}, vec2_u16{46, 30},
		vec2_u16{10, 28}, vec2_u16{42, 32}, vec2_u16{11, 33},
		vec2_u16{45, 27}, vec2_u16{43, 34}, vec2_u16{42, 32},
		vec2_u16{46, 30}, vec2_u16{44, 19}, vec2_u16{43, 34},
		vec2_u16{11, 33}, vec2_u16{21, 35}, vec2_u16{3, 25},
		vec2_u16{42, 32}, vec2_u16{22, 36}, vec2_u16{21, 35},
		vec2_u16{43, 34}, vec2_u16{23, 16}, vec2_u16{22, 36},
		vec2_u16{12, 37}, vec2_u16{36, 38}, vec2_u16{13, 24},
		vec2_u16{39, 39}, vec2_u16{37, 40}, vec2_u16{36, 38},
		vec2_u16{40, 41}, vec2_u16{38, 17}, vec2_u16{37, 40},
		vec2_u16{13, 24}, vec2_u16{33, 42}, vec2_u16{14, 43},
		vec2_u16{36, 38}, vec2_u16{34, 44}, vec2_u16{33, 42},
		vec2_u16{37, 40}, vec2_u16{35, 14}, vec2_u16{34, 44},
		vec2_u16{14, 43}, vec2_u16{30, 35}, vec2_u16{8, 25},
		vec2_u16{33, 42}, vec2_u16{31, 36}, vec2_u16{30, 35},
		vec2_u16{34, 44}, vec2_u16{32, 16}, vec2_u16{31, 36},
		vec2_u16{4, 2}, vec2_u16{39, 39}, vec2_u16{12, 37},
		vec2_u16{26, 1}, vec2_u16{40, 41}, vec2_u16{39, 39},
		vec2_u16{25, 45}, vec2_u16{41, 13}, vec2_u16{40, 41},
		vec2_u16{7, 4}, vec2_u16{48, 29}, vec2_u16{9, 26},
		vec2_u16{29, 46}, vec2_u16{49, 31}, vec2_u16{48, 29},
		vec2_u16{28, 47}, vec2_u16{50, 10}, vec2_u16{49, 31},
		vec2_u16{8, 4}, vec2_u16{29, 46}, vec2_u16{7, 4},
		vec2_u16{30, 46}, vec2_u16{28, 47}, vec2_u16{29, 46},
		vec2_u16{31, 47}, vec2_u16{27, 5}, vec2_u16{28, 47},
		vec2_u16{2, 8}, vec2_u16{23, 11}, vec2_u16{1, 8},
		vec2_u16{24, 11}, vec2_u16{22, 45}, vec2_u16{23, 11},
		vec2_u16{25, 45}, vec2_u16{21, 1}, vec2_u16{22, 45},
		vec2_u16{26, 1}, vec2_u16{4, 2}, vec2_u16{3, 2},
		vec2_u16{14, 3}, vec2_u16{8, 4}, vec2_u16{7, 4},
		vec2_u16{32, 5}, vec2_u16{6, 6}, vec2_u16{5, 6},
		vec2_u16{20, 7}, vec2_u16{2, 8}, vec2_u16{1, 8},
		vec2_u16{27, 5}, vec2_u16{5, 6}, vec2_u16{17, 9},
		vec2_u16{24, 11}, vec2_u16{2, 8}, vec2_u16{20, 12},
		vec2_u16{35, 14}, vec2_u16{18, 18}, vec2_u16{6, 15},
		vec2_u16{38, 17}, vec2_u16{19, 7}, vec2_u16{18, 18},
		vec2_u16{41, 13}, vec2_u16{20, 12}, vec2_u16{19, 7},
		vec2_u16{44, 19}, vec2_u16{15, 21}, vec2_u16{1, 15},
		vec2_u16{47, 20}, vec2_u16{16, 22}, vec2_u16{15, 21},
		vec2_u16{50, 10}, vec2_u16{17, 9}, vec2_u16{16, 22},
		vec2_u16{6, 6}, vec2_u16{18, 23}, vec2_u16{17, 23},
		vec2_u16{18, 23}, vec2_u16{19, 15}, vec2_u16{16, 15},
		vec2_u16{19, 15}, vec2_u16{20, 7}, vec2_u16{15, 7},
		vec2_u16{4, 2}, vec2_u16{12, 24}, vec2_u16{11, 24},
		vec2_u16{12, 24}, vec2_u16{13, 25}, vec2_u16{10, 25},
		vec2_u16{13, 25}, vec2_u16{14, 3}, vec2_u16{9, 3},
		vec2_u16{9, 26}, vec2_u16{48, 29}, vec2_u16{45, 27},
		vec2_u16{48, 29}, vec2_u16{49, 31}, vec2_u16{46, 30},
		vec2_u16{49, 31}, vec2_u16{50, 10}, vec2_u16{47, 20},
		vec2_u16{10, 28}, vec2_u16{45, 27}, vec2_u16{42, 32},
		vec2_u16{45, 27}, vec2_u16{46, 30}, vec2_u16{43, 34},
		vec2_u16{46, 30}, vec2_u16{47, 20}, vec2_u16{44, 19},
		vec2_u16{11, 33}, vec2_u16{42, 32}, vec2_u16{21, 35},
		vec2_u16{42, 32}, vec2_u16{43, 34}, vec2_u16{22, 36},
		vec2_u16{43, 34}, vec2_u16{44, 19}, vec2_u16{23, 16},
		vec2_u16{12, 37}, vec2_u16{39, 39}, vec2_u16{36, 38},
		vec2_u16{39, 39}, vec2_u16{40, 41}, vec2_u16{37, 40},
		vec2_u16{40, 41}, vec2_u16{41, 13}, vec2_u16{38, 17},
		vec2_u16{13, 24}, vec2_u16{36, 38}, vec2_u16{33, 42},
		vec2_u16{36, 38}, vec2_u16{37, 40}, vec2_u16{34, 44},
		vec2_u16{37, 40}, vec2_u16{38, 17}, vec2_u16{35, 14},
		vec2_u16{14, 43}, vec2_u16{33, 42}, vec2_u16{30, 35},
		vec2_u16{33, 42}, vec2_u16{34, 44}, vec2_u16{31, 36},
		vec2_u16{34, 44}, vec2_u16{35, 14}, vec2_u16{32, 16},
		vec2_u16{4, 2}, vec2_u16{26, 1}, vec2_u16{39, 39},
		vec2_u16{26, 1}, vec2_u16{25, 45}, vec2_u16{40, 41},
		vec2_u16{25, 45}, vec2_u16{24, 11}, vec2_u16{41, 13},
		vec2_u16{7, 4}, vec2_u16{29, 46}, vec2_u16{48, 29},
		vec2_u16{29, 46}, vec2_u16{28, 47}, vec2_u16{49, 31},
		vec2_u16{28, 47}, vec2_u16{27, 5}, vec2_u16{50, 10},
		vec2_u16{8, 4}, vec2_u16{30, 46}, vec2_u16{29, 46},
		vec2_u16{30, 46}, vec2_u16{31, 47}, vec2_u16{28, 47},
		vec2_u16{31, 47}, vec2_u16{32, 5}, vec2_u16{27, 5},
		vec2_u16{2, 8}, vec2_u16{24, 11}, vec2_u16{23, 11},
		vec2_u16{24, 11}, vec2_u16{25, 45}, vec2_u16{22, 45},
		vec2_u16{25, 45}, vec2_u16{26, 1}, vec2_u16{21, 1},
	};

	vVertices
	for (size_t i = 0; i < indices.size() / 3; ++i) {
		const size_t index = i * 3;
		const glm::vec3 a = vertices[indices[index]];
		const glm::vec3 b = vertices[indices[index + 1]];
		const glm::vec3 c = vertices[indices[index + 2]];

		vVertices.push_back(a * float(side_legth));
	}
    return mesh_uv();
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
	assert(attribute_buffer_uv2_f32 != NONE); {
		glDeleteBuffers(1, &attribute_buffer_uv2_f32);
		attribute_buffer_uv2_f32 = NONE;
	}
}
