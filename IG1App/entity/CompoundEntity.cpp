#include "CompoundEntity.h"

void CompoundEntity::render(const glm::mat4& basis) const {
	for (Abs_Entity* entity : gObjects) {
		entity->render(basis * mModelMat);
	}
}

void CompoundEntity::update(double time_seconds, double delta_time_seconds) {
	for (Abs_Entity* entity : gObjects) {
		entity->update(time_seconds, delta_time_seconds);
	}
}

void CompoundEntity::load() {
	for (Abs_Entity* entity : gObjects) {
		entity->load();
	}
}

void CompoundEntity::unload() {
	for (Abs_Entity* entity : gObjects) {
		entity->unload();
	}
}