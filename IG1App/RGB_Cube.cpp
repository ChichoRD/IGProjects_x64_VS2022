#include "RGB_Cube.h"

RGB_Cube::RGB_Cube(float l)
{
	mMesh = Mesh::GenerateRGBCube(300);
	load();
}