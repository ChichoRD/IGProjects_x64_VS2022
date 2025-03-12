#include <glm/gtc/matrix_transform.hpp>

#include "showcase_scene.h"
#include "regular_polygon.h"
#include "rgb_rectangle.h"
#include "rgb_triangle.h"
#include "rgb_cube.h"
#include "skibidi.hpp"
#include "box_outline.hpp"
#include "ground.h"
#include "star3.hpp"
#include "glass_parapet.hpp"
#include "photograph.hpp"

void showcase_scene1::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	constexpr static const GLdouble polygon_radius = axis_unit_size * 0.75;
	auto triangle = new regular_polygon{ 3, polygon_radius, glm::dvec4{0.0f, 1.0f, 1.0f, 1.0f} };
	auto circumference = new regular_polygon{ 64, polygon_radius, glm::dvec4{1.0f, 0.0f, 1.0f, 1.0f} };

	gObjects.push_back(triangle);
	gObjects.push_back(circumference);
}


void showcase_scene2::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao
	glFrontFace(GL_CCW);

	constexpr static const GLdouble polygon_radius = axis_unit_size * 0.75;

	constexpr static const GLdouble rectangle_width = polygon_radius * 2.0;
	constexpr static const GLdouble rectangle_height = polygon_radius;
	auto rectangle = new rgb_rectangle{ rectangle_width, rectangle_height, std::array{
		glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
		glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
		glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
		glm::vec4{1.0f, 1.0f, 0.0f, 1.0f},
	}};
	auto triangle = new rgb_triangle{};
	triangle->setModelMat(glm::translate(triangle->modelMat(), { polygon_radius , 0.0f, 0.0f }));

	auto circumference = new regular_polygon{ 64, polygon_radius, glm::dvec4{1.0f, 0.0f, 1.0f, 1.0f} };

	gObjects.push_back(rectangle);
	gObjects.push_back(triangle);
	gObjects.push_back(circumference);
}

void showcase_scene3::init()
{
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao
	
	constexpr static const GLdouble cube_side_length = axis_unit_size * 0.75;
	auto cube = new rgb_cube{ cube_side_length };

	constexpr static const GLdouble half_side_length = cube_side_length * 0.5;
	cube->setModelMat(glm::translate(cube->modelMat(), glm::vec3{ half_side_length, half_side_length, -half_side_length }));

	ground* rizz = new ground(200,200,"./assets/images/skibidi-v2.png");
	gObjects.push_back(rizz);

	gObjects.push_back(cube);
}

void showcase_scene_skibidi::init() {
	Scene::init();
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	constexpr static const GLdouble side_length = axis_unit_size * 0.75;
	constexpr static const GLdouble displacement_scale = side_length;

	// purplish blueish
	constexpr static const glm::vec3 toilet_color{ 0.0f, 0.5f, 1.0f };
	auto gyatt = new skibidi{ glm::vec3{1.0f, 1.0f, 1.0f}, side_length, displacement_scale };

	gObjects.push_back(gyatt);
}

void showcase_scene4::init() {
	Scene::init();
	glEnable(GL_MULTISAMPLE);
	glClearDepth(1.0l);
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	constexpr static const GLdouble side_length = axis_unit_size * 0.75;
	constexpr static const GLdouble displacement_scale = side_length;

	ground* rizz = new ground{axis_unit_size * 4.0f, axis_unit_size * 4.0f, "./assets/images/baldosaC.png"};
	rizz->setModelMat(glm::rotate(rizz->modelMat(), glm::half_pi<float>(), glm::vec3{ 1.0f, 0.0f, 0.0f }));
	gObjects.push_back(rizz);
	
	box_outline *sigma = new box_outline{ "./assets/images/container.jpg", "./assets/images/papelE.png", side_length };
	sigma->setModelMat(glm::translate(sigma->modelMat(), glm::vec3{
		axis_unit_size * 1.0f,
		axis_unit_size * 0.25f,
		axis_unit_size * 1.0f
	}));
	gObjects.push_back(sigma);

	star3 *chad = new star3{ "./assets/images/baldosaP.png", side_length, side_length, float(side_length) * 0.75f, 8 };
	chad->setModelMat(glm::scale(sigma->modelMat(), glm::vec3{ 0.5f, 0.5f, 0.5f }));
	chad->setModelMat(glm::translate(chad->modelMat(), glm::vec3{
		0.0f,
		axis_unit_size * 1.5f,
		0.0f
	}));
	gObjects.push_back(chad);

	photograph *aura = new photograph{ side_length };
	aura->setModelMat(glm::translate(aura->modelMat(), glm::vec3{ -axis_unit_size * 1.0f, axis_unit_size * 0.25f, -axis_unit_size * 1.0f }));
	gObjects.push_back(aura);

	glass_parapet *ohio = new glass_parapet{ side_length };
	ohio->setModelMat(glm::scale(ohio->modelMat(), glm::vec3{ 5.0f, 1.0f, 5.0f }));
	ohio->setModelMat(glm::translate(ohio->modelMat(), glm::vec3{ 0.0f, axis_unit_size * 0.25f, 0.0f }));
	gObjects.push_back(ohio);
}

void showcase_scene4::destroy() {
	Scene::destroy();
	glDisable(GL_MULTISAMPLE);
}
