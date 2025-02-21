#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

class Mesh
{
public:
	static Mesh* generate_rectangle(GLdouble w, GLdouble h, std::array<glm::vec4, 4> &&vertex_colours);
	static Mesh* generate_regular_polygon(GLuint num, GLdouble r);
	static Mesh* generate_regular_polygon(GLuint num, GLdouble r, std::vector<glm::vec4> const& vColors, GLuint primitive);
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* GenerateRGBCube(GLdouble length);
	static Mesh* generateCube(GLdouble length);

	
	Mesh();
	virtual ~Mesh();

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	Mesh(Mesh&& m) noexcept;
	Mesh& operator=(Mesh&& m) noexcept;

	virtual void render() const;

	[[maybe_unused]] inline GLuint size() const { return mNumVertices; }; // number of elements
	[[maybe_unused]] inline std::vector<glm::vec3> const& vertices() const { return vVertices; };
	[[maybe_unused]] inline std::vector<glm::vec4> const& colors() const { return vColors; };

	virtual void load();
	virtual void unload();

	inline void set_primitive(GLuint primitive) { mPrimitive = primitive; };

protected:
	GLuint mPrimitive =
	  GL_TRIANGLES;          // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::vec3> vVertices; // vertex array
	std::vector<glm::vec4> vColors;   // color array
	virtual void draw() const;

	GLuint mVAO;  // vertex array object

private:
	GLuint mVBO;  // vertex buffer object
	GLuint mCBO;  // color buffer object
};

class mesh_uv : public Mesh {
	GLuint attribute_buffer_uv2_f32;
protected:
	std::vector<glm::vec2> vertex_uv2_f32;

public:
	static mesh_uv generate_skibidi_cube(GLfloat side_legth);

	virtual void load() override;
	virtual void unload() override;
};

#endif //_H_Scene_H_
