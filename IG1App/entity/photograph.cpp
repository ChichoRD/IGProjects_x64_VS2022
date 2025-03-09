#include "photograph.hpp"
#include "IG1App.h"

photograph::photograph(const GLfloat side_length)
    : entity_with_texture(false) {
    mMesh = new mesh_uv{mesh_uv::generate_rectangle_uv(side_length, side_length, {0.0f, 0.0f}, {1.0f, 1.0f}, std::array{
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}
    })};
    load();
}

void photograph::update(double time_seconds, double delta_time_seconds) {
    // GLsizei width;
    // GLsizei height;
    // glfwGetFramebufferSize(IG1App::s_ig1app.window(), &width, &height);
    texture.loadColorBuffer(IG1App::s_ig1app.viewPort().width(), IG1App::s_ig1app.viewPort().height(), GL_FRONT);
}

void photograph::render(const glm::mat4 &modelViewMat) const {
    (void)modelViewMat;

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    {
        entity_with_texture::render_with_texture(texture);
    }
    glDepthMask(GL_FALSE);

    glDisable(GL_DEPTH_TEST);
}
