#ifndef GLASS_PARAPET_HPP
#define GLASS_PARAPET_HPP

#include "entity_with_texture.hpp"

class glass_parapet : public entity_with_texture {
public:
    glass_parapet(
        const GLfloat side_length
    );
    void update(double time_seconds, double delta_time_seconds) override;
    void render(const glm::mat4& modelViewMat) const override;
};

#endif