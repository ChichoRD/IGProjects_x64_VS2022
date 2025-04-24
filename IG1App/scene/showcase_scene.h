#pragma once
#include "Scene.h"
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
	virtual void init() override;
};