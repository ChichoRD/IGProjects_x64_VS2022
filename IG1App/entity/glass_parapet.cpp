#include "glass_parapet.hpp"
#include "IG1App.h"
#include <algorithm>

glass_parapet::glass_parapet(const GLfloat side_length)
    : entity_with_texture("./assets/images/windowV.jpg", true) {
    mMesh = new mesh_uv{mesh_uv::generate_box_hull(side_length, {0.0f, 0.0f}, {1.0f, 1.0f}, std::array{
        glm::vec4{1.0f, 1.0f, 1.0f, 0.5f},
        glm::vec4{1.0f, 1.0f, 1.0f, 0.5f},
        glm::vec4{1.0f, 1.0f, 1.0f, 0.5f},
        glm::vec4{1.0f, 1.0f, 1.0f, 0.5f}
    })};
    load();
}

void glass_parapet::update(double time_seconds, double delta_time_seconds) {
    (void)time_seconds;
    (void)delta_time_seconds;
    constexpr static const auto camera_depth =
        [](const glm::mat4 model_view_matrix, const glm::vec3 position) {
        return (model_view_matrix * glm::vec4{position, 1.0f}).z;
    };

    const glm::mat4 model_view_matrix = glm::mat4{IG1App::s_ig1app.camera().viewMat()} * mModelMat;
    std::sort(mMesh->vertices().begin(), mMesh->vertices().end(), [model_view_matrix](const auto& a, const auto& b) {
        return camera_depth(model_view_matrix, a) < camera_depth(model_view_matrix, b);
    });
}

void glass_parapet::render(const glm::mat4 &modelViewMat) const {
    (void)modelViewMat;

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    render_with_texture(texture);

    // glDepthMask(GL_TRUE);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
