#pragma once
#include "ground.h"
#include <array>
#include <glm/ext/matrix_transform.hpp>

ground::ground(int w, int h)
{
    mMesh = Mesh::generate_rectangle(w,h,{
        glm::vec4(1,0,0,1),
        glm::vec4(0,0,1,1),
        glm::vec4(0,1,0,1),
        glm::vec4(0,0,1,1)
    });

    mModelMat = glm::rotate(mModelMat, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    mMesh->set_primitive(GL_LINE_LOOP);
    load();
}