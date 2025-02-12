#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

#include "rgb_cube.h"
#include "Scene.h"

rgb_cube::rgb_cube(float l) : EntityWithColors(), side_length(l)
{
	mMesh = Mesh::GenerateRGBCube(l);
	load();
}

void rgb_cube::update(double time_seconds, double delta_time_seconds)
{
	constexpr static const GLfloat radians_per_second = glm::quarter_pi<GLfloat>();

	const GLfloat angle = radians_per_second * delta_time_seconds;
	const glm::mat4 rotations[3]{
		glm::rotate(glm::identity<glm::mat4>(), angle, glm::vec3{1.0f, 0.0f, 0.0f}),
		glm::rotate(glm::identity<glm::mat4>(), angle, glm::vec3{0.0f, 0.0f, 1.0f}),
		glm::rotate(glm::identity<glm::mat4>(), angle, glm::vec3{0.0f, 1.0f, 0.0f}),
	};


	constexpr static const GLdouble total_animation_time = (glm::pi<GLdouble>() / radians_per_second) * 3.0;
	const GLdouble animation_time = fmod(elapsed_animation_time, total_animation_time);
	const GLdouble animation_time_normalised = animation_time / total_animation_time;

	const size_t rotation_index = floor(animation_time_normalised * 3.0);
	assert(rotation_index < 3 && "fatal error: invalid rotation index");
	if (rotation_index != animation_rotation_index) {
		animation_rotation_index = rotation_index;
		static const glm::mat4 snap_rotations[3]{
			glm::identity<glm::mat4>(),
			glm::rotate(glm::identity<glm::mat4>(), -glm::pi<GLfloat>(), glm::vec3{1.0f, 0.0f, 0.0f}),
			glm::rotate(glm::identity<glm::mat4>(), -glm::pi<GLfloat>(), glm::vec3{0.0f, 0.0f, 1.0f}),
			// glm::rotate(glm::identity<glm::mat4>(), glm::pi<GLfloat>(), glm::vec3{0.0f, 1.0f, 0.0f}),
		};

		setModelMat(
			snap_rotations[rotation_index]
			* glm::translate(glm::identity<glm::mat4>(), snap_translations[rotation_index])
		);
	}
	const glm::mat4 &rotation = rotations[rotation_index];

	setModelMat(
		rotation
		* modelMat()
	);
	elapsed_animation_time += delta_time_seconds;
}
