#include "AdvancedTIE.h"
#include "sphere.h"
#include "Cone.h"
#include "Disk.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

AdvancedTIE::AdvancedTIE() {
	addEntity(new sphere(Scene::axis_unit_size, 12, 12, Scene::cornflower_blue));

	Cone* cone1 = new Cone(100, 50, 50, 4, 12, Scene::cornflower_blue);
	Cone* cone2 = new Cone(100, 50, 50, 4, 12, Scene::cornflower_blue);
	Cone* cone3 = new Cone(100, 50, 50, 4, 12, Scene::cornflower_blue);

	Cone* wing1 = new Cone(100, 50, 50, 4, 4, Scene::cornflower_blue, glm::half_pi<GLfloat>() + glm::pi<GLfloat>() / 6);
	Cone* wing2 = new Cone(100, 50, 50, 4, 4, Scene::cornflower_blue, glm::half_pi<GLfloat>() + glm::pi<GLfloat>() / 6);

	addEntity(cone1);
	addEntity(cone2);
	addEntity(cone3);

	addEntity(wing1);
	addEntity(wing2);
}