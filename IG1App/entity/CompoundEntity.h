#pragma once
#include "Entity.h"
#include <cassert>

class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity() : Abs_Entity() {}
	~CompoundEntity() {};

	inline void addEntity(Abs_Entity* ae) { gObjects.push_back(ae); }
	inline size_t child_count() const { return gObjects.size(); }
	inline const Abs_Entity& get_child(const size_t index) const {
		assert(
			index < gObjects.size()
			&& "fatal error: child index out of bounds"
		);
		return *gObjects.at(index);
	}
	inline Abs_Entity& get_child(const size_t index) {
		assert(
			index < gObjects.size()
			&& "fatal error: child index out of bounds"
		);
		return *gObjects.at(index);
	}

	void render(const glm::mat4& modelViewMat) const override;
	void update(double time_seconds, double delta_time_seconds) override;
	void load();
	void unload();

private:
	std::vector<Abs_Entity*> gObjects;
};

