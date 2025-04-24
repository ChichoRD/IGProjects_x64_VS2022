#pragma once
#include "Entity.h"
class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity() : Abs_Entity() {}
	~CompoundEntity() {};

	inline void addEntity(Abs_Entity* ae) { gObjects.push_back(ae); }

	void render(const glm::mat4& modelViewMat) const override;
	void update(double time_seconds, double delta_time_seconds) override;
	void load();
	void unload();

private:
	std::vector<Abs_Entity*> gObjects;
};

