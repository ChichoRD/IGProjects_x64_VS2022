#ifndef _H_IG1App_H_
#define _H_IG1App_H_

#include <GL/glew.h>     // OpenGL Extension Wrangler
#include <GLFW/glfw3.h>  // OpenGL Library
#include <glm/glm.hpp>   // OpenGL Mathematics

#include "Camera.h"
#include "Scene.h"
#include "Viewport.h"

void ig1_app_on_cursor_position(GLFWwindow* window, double xpos, double ypos);
void ig1_app_on_mouse_button(GLFWwindow* window, int button, int action, int mods);
void ig1_app_on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset);

class IG1App
{
public:
	// static single instance (singleton pattern)
	static IG1App s_ig1app;

	IG1App(IG1App const& J) = delete;         // no copy constructor
	void operator=(IG1App const& J) = delete; // no copy assignment

	// Camera position, view volume and projection
	Camera const& camera() { return *mCamera; };
	// Viewport position and size
	Viewport const& viewPort() { return camera().viewport(); };
	// Graphics objects of the scene
	Scene const& scene() { return *mScenes[mCurrentScene]; };

	// Change to the given scene
	bool changeScene(size_t sceneNr);

	void run();   // the main event processing loop
	void close(); // the application

protected:
	IG1App() = default;
	~IG1App() { close(); };

	void init();
	void iniWinOpenGL();
	void destroy();

	void display() const;                      // the scene
	void update(double time_seconds, double delta_time_seconds);
	void resize(int newWidth, int newHeight);  // the viewport (without changing the scale)
	void key(unsigned int codepoint); // keypress event
	void specialkey(int key, int scancode, int action, int mods); // special keypress event

	// static callbacks
	static void s_display(GLFWwindow*) { s_ig1app.display(); };
	static void s_resize(GLFWwindow*, int newWidth, int newHeight) { s_ig1app.resize(newWidth, newHeight); };
	static void s_key(GLFWwindow* win, unsigned int codepoint) { s_ig1app.key(codepoint); };
	static void s_specialkey(GLFWwindow* win, int key, int scancode, int action, int mods) { s_ig1app.specialkey(key, scancode, action, mods); };

	// Camera position, view volume and projection
	Camera* mCamera = nullptr;
	// Graphics objects are in the scenes
	std::vector<Scene*> mScenes;
	size_t mCurrentScene = 0;

	bool mNeedsRedisplay = true;  // main event processing loop
	bool two_viewport_mode = false;
	GLFWwindow* mWindow = nullptr; // window's handle
	int mWinW = 800;               // window's width
	int mWinH = 600;               // window's height

	bool mUpdateEnabled = true; // update the scene
	double mNextUpdateTime = 0.0; // next update time
	double mUpdateTime = 0.0;     // update period

	glm::dvec2 mouse_position;
	glm::dvec2 previous_mouse_position;
	int mouse_button = -1;
	//glm::dvec2 previous_scroll;
	glm::dvec2 scroll;

	const float ROTATION_SPEED = glm::pi<float>() / 32;

public:
	inline int window_width() const { return mWinW; };
	inline int window_height() const { return mWinH; };

	friend void ig1_app_on_cursor_position(GLFWwindow* window, double xpos, double ypos);
	friend void ig1_app_on_mouse_button(GLFWwindow* window, int button, int action, int mods);
	friend void ig1_app_on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset);
};



#endif //_H_IG1App_H_
