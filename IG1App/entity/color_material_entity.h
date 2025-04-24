#pragma once
#include "single_color_entity.h"
class color_material_entity :
    public single_color_entity
{
    static bool debug_normals_enabled;

    Mesh normals_debug_mesh;
    glm::dvec4 normal_debug_color;
public:
    color_material_entity(const glm::dvec4 color = glm::dvec4{ 1.0f });
	static inline void set_debug_normals_enabled(bool enable) {
        debug_normals_enabled = enable;
    }
	static inline bool get_debug_normals_enabled() {
		return debug_normals_enabled;
	}

    void render(const glm::mat4& modelViewMat) const override;

protected:
	size_t generate_and_load_normals_debug_mesh(const glm::dvec4 debug_color);
};

