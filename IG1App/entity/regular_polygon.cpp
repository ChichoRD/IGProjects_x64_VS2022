#include "regular_polygon.h"
#include "Mesh.h"

regular_polygon::regular_polygon(size_t sides, double radius, glm::dvec4 color)
	: single_color_entity(color)
{
	mMesh = Mesh::generate_regular_polygon(sides, radius);
	load();
}

//regular_polygon::~regular_polygon()
//{
//	unload();
//}
