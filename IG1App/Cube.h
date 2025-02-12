#pragma once
#include "single_color_entity.h"
class Cube : public single_color_entity
{
public:
	Cube();
	void render(const glm::mat4& modelViewMat) const override;
};

