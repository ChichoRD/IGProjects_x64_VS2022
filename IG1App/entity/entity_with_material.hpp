#pragma once
#include "Entity.h"
#include "Material.h"
class entity_with_material :
    public Abs_Entity
{
    Material material;

public:
	inline entity_with_material(const Material material)
		: Abs_Entity()
		, material(material)
	{
		mShader = Shader::get("light");
	}

	void render(const glm::mat4 &basis) const override;
};

