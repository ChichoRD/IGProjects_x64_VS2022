#include <cmath>
#include "skibidi.hpp"

skibidi::skibidi(glm::vec3 color, GLfloat side_length, GLfloat displacement_scale)
    : texture(), color(color), side_length(side_length), displacement_scale(displacement_scale), displacement_factor(0.0f) {
    mShader = Shader::get("skibidi");
    texture.load("../assets/images/skibidi.png");
    mMesh = new mesh_uv(mesh_uv::generate_skibidi_cube(side_length));
    load();
}

void skibidi::render(const glm::mat4 &modelViewMat) const {
    mShader->use();
    upload_model(mModelMat);
    mShader->setUniform("color", color);
    mShader->setUniform("displacement_scale", displacement_scale);
    mShader->setUniform("displacement_factor", displacement_factor);

    texture.bind();
    mMesh->render();
    texture.unbind();
}

void skibidi::update(double time_seconds, double delta_time_seconds) {
    constexpr static const double time_scale = 0.05f;
    displacement_factor = sin(time_seconds * time_scale);
}
