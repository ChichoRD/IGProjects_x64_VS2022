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
public:
    entity_with_texture(const std::string_view texture_path, const std::string_view shader_key, bool modulate = false);
    entity_with_texture(const std::string_view texture_path, bool modulate = false);
    entity_with_texture(const bool modulate);

    [[maybe_unused]] bool &get_modulate();
    [[maybe_unused]] const bool &get_modulate() const;
};

#endif