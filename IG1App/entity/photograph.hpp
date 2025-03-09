#ifndef PHOTOGRAPH_HPP
#define PHOTOGRAPH_HPPs

#include "Entity.h"
#include "Texture.h"
#include "entity_with_texture.hpp"

class photograph : public entity_with_texture {
public:
    photograph(const GLfloat side_length);

    void update(double time_seconds, double delta_time_seconds) override;
    void render(const glm::mat4 &modelViewMat) const override;
      
};

#endif