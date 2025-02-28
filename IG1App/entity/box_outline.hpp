#ifndef BOX_OUTLINE_HPP
#define BOX_OUTLINE_HPP

#include "entity_with_texture.hpp"

class box_outline : public entity_with_texture {
public:
    box_outline(const std::string_view texture_path, const GLfloat side_length);
    void render(const glm::mat4& modelViewMat) const override;
};

#endif