#pragma once
#include "Entity.h"
#include <glm/glm.hpp>

class single_color_entity :
    public Abs_Entity
{
public:
    single_color_entity(glm::dvec4 color = glm::dvec4(1.0f));

    inline glm::dvec4 get_color() const { return color; }
    inline void set_color(glm::dvec4 c) { color = c; }

    virtual void render(const glm::mat4& modelViewMat) const override;
    virtual void upload(const glm::mat4& mModelViewMat) const override;

private:
    glm::dvec4 color;
};

