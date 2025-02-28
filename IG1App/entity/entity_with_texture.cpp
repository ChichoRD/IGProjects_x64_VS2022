#include "entity_with_texture.hpp"

void entity_with_texture::render_with_texture(const glm::mat4 &modelViewMat) const {
    (void)modelViewMat;

    mShader->use();
    upload_model(mModelMat);
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
    : texture(), sampler(0), moulate(modulate)
{
    mShader = Shader::get("texture");
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