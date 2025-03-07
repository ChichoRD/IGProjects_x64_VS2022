#include "box_outline.hpp"

box_outline::box_outline(
    const std::string_view texture_path,
    const std::string_view secondary_texture_path,
    const GLfloat side_length
) : entity_with_texture(texture_path, false), secondary_texture() {
    mMesh = new mesh_uv{mesh_uv::generate_box_hull(side_length, {0.0f, 0.0f}, {1.0f, 1.0f}, std::array{
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
    })};

    secondary_texture.load(secondary_texture_path.data()); {
        secondary_texture.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
        secondary_texture.unbind();
    }
}

void box_outline::render(const glm::mat4 &modelViewMat) const {
    glEnable(GL_CULL_FACE);
    {
        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        entity_with_texture::render_with_texture(modelViewMat, texture);

        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        entity_with_texture::render_with_texture(modelViewMat, secondary_texture);
    }
    glDisable(GL_CULL_FACE);
}