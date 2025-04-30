#pragma once
#include "entity_with_texture.hpp"
class tie_fighter_wing : public entity_with_texture
{
public:
	tie_fighter_wing();
	~tie_fighter_wing() {}

	void render(const glm::mat4& basis) const override;
	void update(double time_seconds, double delta_time_seconds) override;
};

