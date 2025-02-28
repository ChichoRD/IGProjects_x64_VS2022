#include "box_outline.hpp"

box_outline::box_outline(const std::string_view texture_path, const GLfloat side_length)
    : entity_with_texture(texture_path, false) {
    mMesh = new mesh_uv{mesh_uv::generate_box_hull(side_length, {0.0f, 0.0f}, {1.0f, 1.0f}, std::array{
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
    })};
}

void box_outline::render(const glm::mat4 &modelViewMat) const {
    entity_with_texture::render_with_texture(modelViewMat);
}