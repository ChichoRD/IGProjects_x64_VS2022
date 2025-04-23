#include "indexed_box.h"
#include "../mesh/IndexMesh.h"

indexed_box::indexed_box(const GLdouble side_length, const glm::dvec4 color)
	: color_material_entity{color} {
	mMesh = IndexMesh::generate_indexed_box(side_length);
	mMesh->load();
}
