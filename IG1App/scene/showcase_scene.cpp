#include <glm/gtc/matrix_transform.hpp>

#include "showcase_scene.h"
#include "regular_polygon.h"
#include "rgb_rectangle.h"
#include "rgb_triangle.h"
#include "rgb_cube.h"
#include "skibidi.hpp"
#include "box_outline.hpp"

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
	glClearColor(0.6f, 0.7f, 0.8f, 1.0f); // cornflower blue lmao

	constexpr static const GLdouble side_length = axis_unit_size * 0.75;
	constexpr static const GLdouble displacement_scale = side_length;

	// purplish blueish
	constexpr static const glm::vec3 toilet_color{ 0.0f, 0.5f, 1.0f };
	box_outline *sigma = new box_outline{ "models/box_outline.png", side_length };
	gObjects.push_back(sigma);
}