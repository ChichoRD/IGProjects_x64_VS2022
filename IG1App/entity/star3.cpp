#include "star3.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
    (void)modelViewMat;

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    entity_with_texture::render_with_texture(texture);

    const glm::mat4 model = glm::scale(mModelMat, glm::vec3{-1.0f, -1.0f, -1.0f});
    entity_with_texture::render_with_texture_and_model(texture, model);

    glDepthMask(GL_FALSE);
    
    glDisable(GL_DEPTH_TEST);
}

void star3::update(double time_seconds, double delta_time_seconds) {
    (void)time_seconds;
    constexpr static const GLfloat roll_speed = glm::two_pi<GLfloat>() * 0.25f;
    constexpr static const GLfloat yaw_speed = glm::two_pi<GLfloat>() * 0.5f;

    mModelMat = glm::rotate(mModelMat, yaw_speed * float(delta_time_seconds), glm::vec3{0.0f, 1.0f, 0.0f});
    const glm::vec3 forward = mModelMat * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f};
    mModelMat = glm::rotate(mModelMat, roll_speed * float(delta_time_seconds), forward);
}
