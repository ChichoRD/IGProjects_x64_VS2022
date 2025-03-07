#ifndef START3_HPP
#define START3_HPP

#include "entity_with_texture.hpp"
#include <string_view>


class star3 : public entity_with_texture {
public:
    star3(
        const std::string_view texture_path,
        const GLfloat side_length,
        const GLuint base_vertices
    );
    void render(const glm::mat4& modelViewMat) const override;
};

#endif