#include "Cube.h"

Cube::Cube()
{
	mMesh = Mesh::generateCube(300);
	load();
}