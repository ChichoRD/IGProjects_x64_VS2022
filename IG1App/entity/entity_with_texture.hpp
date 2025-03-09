#ifndef ENTITY_WITH_TEXTURE_HPP
#define ENTITY_WITH_TEXTURE_HPP

#include "Entity.h"
#include "Texture.h"
#include <string_view>

class entity_with_texture : public Abs_Entity {
protected:
    Texture texture;
    GLuint sampler;
    bool moulate;
    
    void render_with_texture_and_model(const Texture &texture, const glm::mat4 &model_matrix) const;
    void render_with_texture(const Texture &texture) const;
public:
    entity_with_texture(const std::string_view texture_path, const std::string_view shader_key, bool modulate = false);
    entity_with_texture(const std::string_view texture_path, bool modulate = false);
    inline entity_with_texture(bool modulate) : texture(), sampler(0), moulate(modulate) {
        mShader = Shader::get("texture");
        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    [[maybe_unused]] inline bool &get_modulate() { return moulate; }
    [[maybe_unused]] inline const bool &get_modulate() const { return moulate; }
};

#endif