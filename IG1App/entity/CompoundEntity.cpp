#include "CompoundEntity.h"
#include <type_traits>
#include <cassert>
#include <cstdlib>

void CompoundEntity::addEntity(Abs_Entity* ae) {
	if (ae == nullptr) {
		assert(false && "error: entity may not be null");
		std::exit(EXIT_FAILURE);
	}

	std::unique_ptr<Abs_Entity> entity{ ae };
	gObjects.push_back(std::move(entity));
}

size_t CompoundEntity::child_count() const {
	return gObjects.size();
}

const Abs_Entity& CompoundEntity::get_child(const size_t index) const {
	assert(
		index < gObjects.size()
		&& "fatal error: child index out of bounds"
	);
	return *gObjects.at(index);
}

Abs_Entity& CompoundEntity::get_child(const size_t index) {
	assert(
		index < gObjects.size()
		&& "fatal error: child index out of bounds"
	);
	return *gObjects.at(index);
}


void CompoundEntity::render(const glm::mat4& basis) const {
	for (const std::unique_ptr<Abs_Entity> &entity : gObjects) {
		entity->render(basis * mModelMat);
	}
}

void CompoundEntity::update(double time_seconds, double delta_time_seconds) {
	for (std::unique_ptr<Abs_Entity>& entity : gObjects) {
		entity->update(time_seconds, delta_time_seconds);
	}
}

void CompoundEntity::load() {
	for (std::unique_ptr<Abs_Entity> &entity : gObjects) {
		entity->load();
	}
}

void CompoundEntity::unload() {
	for (std::unique_ptr<Abs_Entity> &entity : gObjects) {
		entity->unload();
	}
}