#pragma once
#include "Entity.h"
#include <glm/glm.hpp>

class single_color_entity :
    public Abs_Entity
{
public:
    single_color_entity(glm::dvec4 color = glm::dvec4(1.0f));

    [[maybe_unused]] glm::dvec4 get_color() const;
    [[maybe_unused]] void set_color(glm::dvec4 c);

    virtual void render(const glm::mat4& modelViewMat) const override;

private:
    glm::dvec4 color;
};

