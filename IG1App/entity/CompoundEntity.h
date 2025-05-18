#pragma once
#include "Entity.h"
#include <cassert>
#include <memory>

class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity() : Abs_Entity() {}
	~CompoundEntity() {};

	void addEntity(Abs_Entity* ae);
	size_t child_count() const;
	const Abs_Entity& get_child(const size_t index) const;
	Abs_Entity& get_child(const size_t index);

	void render(const glm::mat4& modelViewMat) const override;
	void update(double time_seconds, double delta_time_seconds) override;
	void load();
	void unload();

private:
	std::vector<std::unique_ptr<Abs_Entity>> gObjects;
};

