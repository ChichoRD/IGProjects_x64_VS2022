#include "rgb_triangle.h"

rgb_triangle::rgb_triangle()
	: EntityWithColors()
{
	mMesh = Mesh::generate_regular_polygon(3, 200, std::vector{
		glm::vec4(1,0,0,1),
		glm::vec4(0,1,0,1),
		glm::vec4(0,0,1,1)
		}, GL_TRIANGLES);
	load();
}