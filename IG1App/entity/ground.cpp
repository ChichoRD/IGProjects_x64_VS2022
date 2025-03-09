#pragma once
#include "ground.h"
#include <array>
#include <glm/ext/matrix_transform.hpp>

ground::ground(int w, int h, const std::string_view texturePath) : entity_with_texture(texturePath)
{
    texture.setWrap(GL_REPEAT);
    mMesh = new mesh_uv{mesh_uv::generate_rectangle_uv(w,h,glm::vec2(0,0),glm::vec2(4, 4),{
        glm::vec4(1,0,0,1),
        glm::vec4(0,0,1,1),
        glm::vec4(0,1,0,1),
        glm::vec4(0,0,1,1)
    })};

    mModelMat = glm::rotate(mModelMat, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    // mMesh->set_primitive(GL_LINE_LOOP);
    load();
}

void ground::render(const glm::mat4& modelViewMat) const {
    render_with_texture(texture);
}