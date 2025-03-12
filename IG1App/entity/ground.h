#pragma once
#include "Entity.h"
#include "entity_with_texture.hpp"

class ground : public entity_with_texture
{
public:
    ground(GLfloat w, GLfloat h, const std::string_view texturePath);
    void render(const glm::mat4& modelViewMat) const override;
};