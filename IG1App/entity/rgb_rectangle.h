#pragma once
#include "Entity.h"
class rgb_rectangle :
    public EntityWithColors
{
public:
    explicit rgb_rectangle(GLdouble w, GLdouble h, std::array<glm::vec4, 4> &&vertex_colours);

    virtual void render(const glm::mat4& modelViewMat) const override;
    [[maybe_unused]] inline Mesh* getMesh() { return mMesh; }
};

