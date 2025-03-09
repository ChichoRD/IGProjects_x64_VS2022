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
    entity_with_texture(const std::string_view texture_path, bool modulate = false);

    inline bool &get_modulate() { return moulate; }
    inline const bool &get_modulate() const { return moulate; }
};

#endif