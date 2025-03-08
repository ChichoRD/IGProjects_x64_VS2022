#include "star3.hpp"

star3::star3(
    const std::string_view texture_path,
    const GLfloat height,
    const GLfloat outter_radius,
    const GLfloat inner_radius,
    const GLuint inner_vertex_count
) : entity_with_texture(texture_path, false) {
    mMesh = new mesh_uv{mesh_uv::generate_stellated_pyramid(
        height,
        outter_radius,
        inner_radius,
        inner_vertex_count
    )};
    load();
}

void star3::render(const glm::mat4 &modelViewMat) const {
    entity_with_texture::render_with_texture(modelViewMat, texture);
}
