#ifndef START3_HPP
#define START3_HPP

#include "entity_with_texture.hpp"
#include <string_view>


class star3 : public entity_with_texture {
public:
    star3(
        const std::string_view texture_path,
        const GLfloat height,
        const GLfloat outter_radius,
        const GLfloat inner_radius,
        const GLuint inner_vertex_count
    );
    void render(const glm::mat4& modelViewMat) const override;
};

#endif