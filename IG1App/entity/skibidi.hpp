#ifndef SKIBIDI_HPP
#define SKIBIDI_HPP

#include "Entity.h"
#include "Texture.h"

class skibidi : public Abs_Entity {
    Texture texture;

    glm::vec3 color;
    GLfloat side_length;
    GLfloat displacement_scale;
    GLfloat displacement_factor;

public:
    skibidi(glm::vec3 color, GLfloat side_length, GLfloat displacement_scale);

    virtual void render(const glm::mat4& modelViewMat) const override;
    virtual void update(double time_seconds, double delta_time_seconds) override;
};

#endif