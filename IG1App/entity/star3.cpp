#include "star3.hpp"

star3::star3(const std::string_view texture_path, const GLfloat side_length, const GLuint base_vertices)
    : entity_with_texture(texture_path, false) {
    mMesh = new mesh_uv{mesh_uv::generate_stellated_pyramid(
        400.0f,
        side_length,
        side_length * 0.5,
        base_vertices
    )};
    load();
}

void star3::render(const glm::mat4 &modelViewMat) const {
    entity_with_texture::render_with_texture(modelViewMat, texture);
}
