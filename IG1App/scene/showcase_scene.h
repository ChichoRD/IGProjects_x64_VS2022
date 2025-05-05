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

    PosLight* pos_light;
    SpotLight* spot_light;
    
    virtual void init() override;
    virtual void on_key_pressed(const uint32_t key) override;

public:
    void rotate_tie(const float radians);
    void orbit_tie(const float arc_length);
};

class showcase_scene0 : public Scene {
    virtual void init() override;
};