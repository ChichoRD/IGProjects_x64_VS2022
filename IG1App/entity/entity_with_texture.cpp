#include "entity_with_texture.hpp"

void entity_with_texture::render_with_texture_and_model(const Texture &texture, const glm::mat4 &model_matrix) const {
    mShader->use();
    upload_model(model_matrix);
    mShader->setUniform("modulate", moulate);

    {
        texture.bind();
        GLuint texture_unit = mShader->get_location("ourTexture");
        glBindSampler(texture_unit, sampler);
        mMesh->render();

        glBindSampler(texture_unit, 0);
        texture.unbind();
    }
}


entity_with_texture::entity_with_texture(const std::string_view texture_path, bool modulate)
    : entity_with_texture(texture_path, "texture", modulate) {}

entity_with_texture::entity_with_texture(const bool modulate) : texture(), sampler(0), moulate(modulate) {
    mShader = Shader::get("texture");
    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

[[maybe_unused]]
bool& entity_with_texture::get_modulate() { return moulate; }
[[maybe_unused]]
const bool& entity_with_texture::get_modulate() const { return moulate; }

entity_with_texture::entity_with_texture(const std::string_view texture_path, const std::string_view shader_key, bool modulate)
    : texture(), sampler(0), moulate(modulate) {
    mShader = Shader::get(shader_key.data());
    texture.load(texture_path.data()); {
        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        texture.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
        texture.unbind();
    }
}
