#pragma once
#include <cstdint>
#include "Entity.h"

class rgb_cube : public EntityWithColors
{
	double elapsed_animation_time = 0.0;
	const float side_length;
	uint8_t animation_rotation_index = 0;

	const std::array<glm::vec3, 3> snap_translations{
		glm::vec3{ side_length * 0.5,	side_length * 0.5,		-side_length  * 0.5,	},
		glm::vec3{ side_length * 0.5,	side_length * 0.5,		-side_length  * 0.5,	},
		glm::vec3{ side_length * 0.5,	-side_length  * 0.5,	side_length * 0.5,		},
	};
public:
	rgb_cube(float l);
	virtual void update(double time_seconds, double delta_time_seconds) override;
};

