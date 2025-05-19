#pragma once
#include "CompoundEntity.h"
#include "Light.h"
#include "Scene.h"

class CompoundTIE :
    public CompoundEntity
{
	std::vector<size_t> tie_child_path;
	SpotLight *spotlight;

	CompoundEntity* tie_anchor;
	CompoundEntity* tie_planet_anchor;
public:
	CompoundTIE(Scene &scene);
	void update(double time_seconds, double delta_time_seconds) override;
	glm::mat4 compute_tie_transform() const;

	void rotate_tie(const float radians);
	void orbit_tie(const float arc_length);

	SpotLight& get_spotlight();
	const SpotLight& get_spotlight() const;
};

