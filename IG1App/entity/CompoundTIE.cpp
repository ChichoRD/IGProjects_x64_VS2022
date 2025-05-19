#include "CompoundTIE.hpp"
#include "AdvancedTIE.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <limits>
#include <memory>

CompoundTIE::CompoundTIE(Scene& scene) : tie_child_path{} {
	AdvancedTIE* tie = new AdvancedTIE();
	tie->setModelMat(
		glm::rotate(
			glm::scale(glm::identity<glm::mat4>(), glm::vec3{ 0.2f }),
			glm::pi<float>(),
			glm::vec3{ 0.0f, 1.0f, 0.0f }
		)
	);

	CompoundEntity* tie_anchor = new CompoundEntity();
	tie_child_path.push_back(tie_anchor->child_count());
	tie_anchor->addEntity(tie);
	tie_anchor->setModelMat(
		glm::translate(
			glm::identity<glm::mat4>(), glm::vec3{ 0.0f, Scene::axis_unit_size, 0.0f }
		)
	);

	CompoundEntity* tie_planet_anchor = new CompoundEntity();
	tie_child_path.push_back(tie_planet_anchor->child_count());
	tie_planet_anchor->addEntity(tie_anchor);

	tie_child_path.push_back(child_count());
	addEntity(tie_planet_anchor);
	this->tie_anchor = tie_anchor;
	this->tie_planet_anchor = tie_planet_anchor;

	const glm::mat4 tie_transform = compute_tie_transform();
	std::unique_ptr<SpotLight> tie_spotlight{
		std::make_unique<SpotLight>(
			glm::vec3{glm::column(tie_transform, 3)},
			static_cast<int>(scene.get_lights().size())
		)
	};
	tie_spotlight->setDirection(glm::normalize(glm::vec3{
		tie_transform * glm::vec4{ 0.0f, -1.0f, 0.0f, 0.0f }
		}));
	tie_spotlight->setCutoff(30.f, 60.0f);
	spotlight = tie_spotlight.get();
	scene.get_lights().push_back(std::move(tie_spotlight));
}

void CompoundTIE::update(double time_seconds, double delta_time_seconds) {
	const glm::mat4 tie_transform = compute_tie_transform();
	SpotLight& tie_spotlight{ *spotlight };
	tie_spotlight.setPosition(glm::vec3{ glm::column(tie_transform, 3) });
	tie_spotlight.setDirection(glm::normalize(glm::vec3{
		tie_transform * glm::vec4{ 0.0f, -1.0f, 0.0f, 0.0f }
	}));
}

glm::mat4 CompoundTIE::compute_tie_transform() const {
	glm::mat4 result{ modelMat() };
	const CompoundEntity* parent = this;
	for (ptrdiff_t i = ptrdiff_t(tie_child_path.size()) - 1; i >= 1; --i) {
		const Abs_Entity& child = parent->get_child(tie_child_path.at(i));
		result *= child.modelMat();
		parent = static_cast<const CompoundEntity*>(&child);
	}

	if (!tie_child_path.empty()) {
		return result * parent->get_child(tie_child_path.at(0)).modelMat();
	}
	else {
		return result;
	}

}

void CompoundTIE::rotate_tie(const float radians) {
	tie_planet_anchor->setModelMat(glm::rotate(
		tie_planet_anchor->modelMat(),
		radians,
		glm::vec3{ 0.0f, 1.0f, 0.0f }
	));
}

void CompoundTIE::orbit_tie(const float arc_length){
	//tie_anchor->setModelMat(glm::rotate(tie_anchor->modelMat(), arc_length / float(Scene::axis_unit_size), glm::vec3{ 1.0f, 0.0f, 0.0f }));
	glm::vec4 axis = tie_anchor->modelMat() * glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f };
	tie_planet_anchor->setModelMat(glm::rotate(tie_planet_anchor->modelMat(), arc_length / float(Scene::axis_unit_size), glm::vec3{ axis }));
}

SpotLight& CompoundTIE::get_spotlight() {
	return *spotlight;
}
const SpotLight& CompoundTIE::get_spotlight() const {
	return *spotlight;
}
