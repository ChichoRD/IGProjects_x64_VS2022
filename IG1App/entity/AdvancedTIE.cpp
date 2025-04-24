#include "AdvancedTIE.h"
#include "sphere.h"
#include "Cone.h"
#include "Disk.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

AdvancedTIE::AdvancedTIE() {
	addEntity(new sphere(Scene::axis_unit_size/4, 12, 12, Scene::cornflower_blue));

	Cone* cone1 = new Cone(100, 25, 25, 100, 12, Scene::cornflower_blue);
	Cone* cone2 = new Cone(100, 25, 25, 100, 12, Scene::cornflower_blue);
	Cone* cone3 = new Cone(100, 25, 25, 100, 12, Scene::cornflower_blue);

	cone1->setModelMat(glm::rotate(cone1->modelMat(), glm::half_pi<GLfloat>(), glm::vec3(0,0,1)));
	cone1->setModelMat(glm::translate(glm::identity<glm::mat4>(), glm::vec3(Scene::axis_unit_size / 3, 0, 0)) * cone1->modelMat());

	cone2->setModelMat(glm::rotate(cone2->modelMat(), glm::half_pi<GLfloat>(), glm::vec3(0, 0, -1)));
	cone2->setModelMat(glm::translate(glm::identity<glm::mat4>(), glm::vec3(-Scene::axis_unit_size / 3, 0, 0)) * cone2->modelMat());

	cone3->setModelMat(glm::rotate(cone3->modelMat(), glm::half_pi<GLfloat>(), glm::vec3(1, 0, 0)));
	cone3->setModelMat(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, -Scene::axis_unit_size / 3)) * cone3->modelMat());

	Cone* wing1 = new Cone(100, 150, 150, 4, 5, Scene::cornflower_blue, glm::half_pi<GLfloat>() + glm::pi<GLfloat>() / 6);
	Cone* wing2 = new Cone(100, 150, 150, 4, 5, Scene::cornflower_blue, glm::half_pi<GLfloat>() + glm::pi<GLfloat>() / 6);

	wing1->setModelMat(glm::rotate(wing1->modelMat(), glm::quarter_pi<GLfloat>(), glm::vec3(0, 0, 1)));
	wing1->setModelMat(glm::rotate(wing1->modelMat(), glm::half_pi<GLfloat>(), glm::vec3(1, 0, 0)));
	wing1->setModelMat(glm::translate(glm::identity<glm::mat4>(), glm::vec3(-15, 0, -33.3333333469)) * wing1->modelMat());

	wing2->setModelMat(glm::rotate(wing2->modelMat(), glm::half_pi<GLfloat>() + glm::quarter_pi<GLfloat>(), glm::vec3(0, 0, 1)));
	wing2->setModelMat(glm::rotate(wing2->modelMat(), -glm::half_pi<GLfloat>(), glm::vec3(1, 0, 0)));
	wing2->setModelMat(glm::translate(glm::identity<glm::mat4>(), glm::vec3(15, 0, 33.3333333469)) * wing2->modelMat());

	addEntity(cone1);
	addEntity(cone2);
	addEntity(cone3);

	addEntity(wing1);
	addEntity(wing2);
}