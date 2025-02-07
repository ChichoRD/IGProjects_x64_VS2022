#pragma once
#include "Entity.h"
class rgb_triangle : public EntityWithColors
{
public:
	rgb_triangle();
	void render(const glm::mat4& modelViewMat) const override;
	virtual void update() override;
};

