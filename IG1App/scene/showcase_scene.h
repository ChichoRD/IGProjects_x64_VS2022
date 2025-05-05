#pragma once
#include "Scene.h"
#include "../entity/CompoundEntity.h"
class showcase_scene1 :
    public Scene
{
    virtual void init() override;
};

class showcase_scene2 :
    public Scene
{
	virtual void init() override;
};

class showcase_scene3 :
    public Scene
{
	virtual void init() override;
};

class showcase_scene_skibidi : public Scene {
    virtual void init() override;
};

class showcase_scene4 : public Scene {
    virtual void init() override;
    virtual void destroy() override;
};

class showcase_scene5 : public Scene {
    virtual void init() override;
};

class showcase_scene6 : public Scene {
	virtual void init() override;
	virtual void destroy() override;
};

class showcase_scene7 : public Scene {
    virtual void init() override;
};

class showcase_scene8 : public Scene {
    CompoundEntity *tie_anchor;
    CompoundEntity *tie_planet_anchor;
	std::vector<size_t> tie_child_path;

	size_t tie_spotlight_index;
    size_t spotlight_index;
	size_t positional_light_index;
    
    virtual void init() override;
    virtual void on_key_pressed(const uint32_t key) override;

	template <typename LightType>
    LightType& get_light(const size_t light_index) {
		assert(
			light_index < lights.size()
			&& "fatal error: light index out of bounds"
		);
		Light* const light{ lights.at(light_index).get() };
		assert(
			light != nullptr
			&& "fatal error: light is null"
		);
		LightType* const light_type{ dynamic_cast<LightType*>(light) };
		assert(
			light_type != nullptr
			&& "fatal error: light is not of the expected type"
		);
		return *light_type;
    }
	template <typename LightType>
	const LightType& get_light(const size_t light_index) const {
		assert(
			light_index < lights.size()
			&& "fatal error: light index out of bounds"
		);
		const Light* const light{ lights.at(light_index).get() };
		assert(
			light != nullptr
			&& "fatal error: light is null"
		);
		const LightType* const light_type{ dynamic_cast<const LightType*>(light) };
		assert(
			light_type != nullptr
			&& "fatal error: light is not of the expected type"
		);
		return *light_type;
	}

	inline SpotLight& get_spotlight() {
		return get_light<SpotLight>(spotlight_index);
	}
	inline const SpotLight& get_spotlight() const {
		return get_light<SpotLight>(spotlight_index);
	}
	inline PosLight& get_positional_light() {
		return get_light<PosLight>(positional_light_index);
	}
	inline const PosLight& get_positional_light() const {
		return get_light<PosLight>(positional_light_index);
	}
	inline SpotLight& get_tie_spotlight() {
		return get_light<SpotLight>(tie_spotlight_index);
	}
	inline const SpotLight& get_tie_spotlight() const {
		return get_light<SpotLight>(tie_spotlight_index);
	}

	glm::mat4 compute_tie_transform() const;

public:
    showcase_scene8();

    void rotate_tie(const float radians);
    void orbit_tie(const float arc_length);

	void update(double time_seconds, double delta_time_seconds) override;
};

class showcase_scene0 : public Scene {
    virtual void init() override;
};