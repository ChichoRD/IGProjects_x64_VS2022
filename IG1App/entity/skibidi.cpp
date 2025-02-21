#include <cmath>
#include "skibidi.hpp"

skibidi::skibidi(glm::vec3 color, GLfloat side_length, GLfloat displacement_scale)
    : texture(), color(color), side_length(side_length), displacement_scale(displacement_scale), displacement_factor(0.0f) {
    mShader = Shader::get("skibidi");
    texture.load("./assets/images/skibidi.png"); {
        texture.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST
        texture.unbind();
    }

    mMesh = new mesh_uv(mesh_uv::generate_skibidi_cube(side_length));
    load();
}

void skibidi::render(const glm::mat4 &modelViewMat) const {
    (void)modelViewMat;

    mShader->use();
    upload_model(mModelMat);
    mShader->setUniform("color", glm::vec4{color, 1.0f});
    mShader->setUniform("displacement_scale", displacement_scale);
    mShader->setUniform("displacement_factor", displacement_factor);

    {
        texture.bind();
        mMesh->render();
        texture.unbind();
    }
}

void skibidi::update(double time_seconds, double delta_time_seconds) {
    (void)delta_time_seconds;

    constexpr static const float time_scale = 2.0f;
    displacement_factor = sinf(float(time_seconds) * time_scale) * 0.5f + 0.5f;
}
