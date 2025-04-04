#include "IG1App.h"

#include <iostream>
#include <thread>
//#include "scene_e1.h"
//#include "scene_e8.h"
//#include "scene_e13.h"
#include "showcase_scene.h"

using namespace std;

// static single instance (singleton pattern)
IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
{
	switch (id) {
	case 131204: // GL_DEBUG_SOURCE_API_ARB - Texture level 0 ndef
	case 131185: // NVidia Driver Notification - Video memory usage
		break;
	default: {
		const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
			? "\x1b[31m[ERROR]\x1b[0m "
			: "\x1b[33m[WARNING]\x1b[0m ";
		cout << prefix << message << endl;

		//　オイオイオイ
		assert(false && "error: opengl reached an ill state");
		break;
	}
	}
}


void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true); // stops main loop
}

#define FRAME_DURATION (1.0 / 30.0)

void
IG1App::run() // enters the main event processing loop
{
	if (mWindow == 0) // if not intilialized
		init();

	double last_update_time = glfwGetTime();
	// IG1App main loop
	while (!glfwWindowShouldClose(mWindow)) {
		mUpdateTime = glfwGetTime();
		mNextUpdateTime = mUpdateTime + FRAME_DURATION;

		const double previous_update_time = last_update_time;
		last_update_time = mUpdateTime;
		if (mUpdateEnabled) {
			update(mUpdateTime, mUpdateTime - previous_update_time);
			if (mNeedsRedisplay) {
				display();
				mNeedsRedisplay = false;
			}

			const double time_to_update = (std::max)(mNextUpdateTime - glfwGetTime(), 0.0);
			if (time_to_update > 0.0) {
				glfwWaitEventsTimeout(time_to_update);
				const double remaining_frame_time = (std::max)(mNextUpdateTime - glfwGetTime(), 0.0);
				if (remaining_frame_time > 0.0) {
					std::this_thread::sleep_for(std::chrono::duration<double>(remaining_frame_time));
				}
			}
		} else {

			// Redisplay the window if needed
			if (mNeedsRedisplay) {
				display();
				mNeedsRedisplay = false;
			}

			// Stop and wait for new events
			glfwWaitEvents();
		}
	}

	destroy();
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mCamera = new Camera(Viewport(mWinW, mWinH));
	mScenes.push_back(new showcase_scene1);
	mScenes.push_back(new showcase_scene2);
	mScenes.push_back(new showcase_scene3);
	mScenes.push_back(new showcase_scene_skibidi);
	mScenes.push_back(new showcase_scene4);

	mCamera->set_position(glm::dvec3(0.0, 0.0, -10.0));
	mCamera->look_at(glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 1.0, 0.0)); 
	mScenes.front()->init();
}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS

	// Create window with its associated OpenGL context, return window's pointer
	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);

	if (mWindow == nullptr) {
		glfwTerminate();
		throw std::logic_error("cannot create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);

	// Initialize OpenGL extension library
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw std::logic_error("Error while loading extensions: "s +
		                       reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Callback registration
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetCursorPosCallback(mWindow, ig1_app_on_cursor_position);
	glfwSetMouseButtonCallback(mWindow, ig1_app_on_mouse_button);
	glfwSetScrollCallback(mWindow, ig1_app_on_mouse_scroll);

	// Error message callback (all messages)
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void ig1_app_on_cursor_position(GLFWwindow* window, double xpos, double ypos) {
	IG1App &app = *static_cast<IG1App*>(glfwGetWindowUserPointer(window));

	app.previous_mouse_position = app.mouse_position;
	app.mouse_position = glm::dvec2(xpos, ypos);
}

void ig1_app_on_mouse_button(GLFWwindow* window, int button, int action, int mods) {
	IG1App &app = *static_cast<IG1App*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		app.mouse_button = button;
	} else if (action == GLFW_RELEASE) {
		app.mouse_button = -1;
	}
}

void ig1_app_on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset) {
	IG1App &app = *static_cast<IG1App*>(glfwGetWindowUserPointer(window));

	//app.previous_scroll = app.scroll;
	app.scroll = glm::dvec2{xoffset, yoffset};
}

void
IG1App::destroy()
{ // release memory and resources
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	delete mCamera;
	mCamera = nullptr;

	glfwTerminate();
}

static void ig1_app_render_two_viewport(const Scene &scene, const Camera camera) {
	Camera left_camera{camera};
	Camera right_camera{camera};

	left_camera.setSize(camera.viewport().width() / 2, camera.viewport().height());
	right_camera.setSize(camera.viewport().width() / 2, camera.viewport().height());

	camera_set_cenital(right_camera, glm::dvec3(0.0, 400.0, 0.0));

	left_camera.viewport().setPos(0, 0); // left viewport
	left_camera.viewport().setSize(camera.viewport().width() / 2, camera.viewport().height());
	scene.render(left_camera); // uploads the viewport and camera to the GPU

	right_camera.viewport().setPos(camera.viewport().width() / 2, 0); // right viewport
	right_camera.viewport().setSize(camera.viewport().width() / 2, camera.viewport().height());
	scene.render(right_camera); // uploads the viewport and camera to the GPU

	glViewport(0, 0, camera.viewport().width(), camera.viewport().height());
}

void
IG1App::display() const
{ // double buffering

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer
	glDepthMask(GL_FALSE);

	if (two_viewport_mode) {
		ig1_app_render_two_viewport(*mScenes[mCurrentScene], *mCamera);
	} else {
		mScenes[mCurrentScene]->render(*mCamera); // uploads the viewport and camera to the GPU
	}

	glfwSwapBuffers(mWindow); // swaps the front and back buffer
}

void IG1App::update(double time_seconds, double delta_time_seconds) {

	glm::dvec2 mouse_displacement = mouse_position - previous_mouse_position;
	if (mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
		// TODO: mouse wheel and customize
		mCamera->orbit_xz(
			mouse_displacement.x * 0.01f,
			mouse_displacement.y * 0.1f,
			float(1 << 9)
		);
	} else if (mouse_button == GLFW_MOUSE_BUTTON_RIGHT) {
		mCamera->move_lr(mouse_displacement.x);
		mCamera->move_ud(-mouse_displacement.y);
	}

	glm::dvec2 scroll_delta = scroll;
	scroll = {0.0, 0.0};
	constexpr static const glm::dvec2 forward{0.0, 1.0};
	const double scroll_delta_proj_sqr = scroll_delta.x * forward.x + scroll_delta.y * forward.y;
	if(glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
		mCamera->setScale(scroll_delta_proj_sqr * 0.01);
	} else {
		mCamera->move_fb(float(scroll_delta_proj_sqr * float(1 << 6)));
	}

	mScenes[mCurrentScene]->update(time_seconds, delta_time_seconds);
	mNeedsRedisplay = true;
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	auto &&viewport = mCamera->viewport();
	viewport.setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(viewport.width(), viewport.height());
}

void
IG1App::key(unsigned int key)
{
	bool need_redisplay = true;

	constexpr static const float camera_frame_displacement = 10.0f;
	switch (key) {
		case '+':
			mCamera->setScale(+0.01); // zoom in  (increases the scale)
			break;
		case '-':
			mCamera->setScale(-0.01); // zoom out (decreases the scale)
			break;
		case 'u':
			mUpdateEnabled = !mUpdateEnabled;
			break;
		case 'w':
			mCamera->move_fb(camera_frame_displacement);
			break;
		case 's':
			mCamera->move_fb(-camera_frame_displacement);
			break;
		case 'a':
			mCamera->move_lr(-camera_frame_displacement);
			break;
		case 'd':
			mCamera->move_lr(camera_frame_displacement);
			break;
		case 'q':
			mCamera->move_ud(-camera_frame_displacement);
			break;
		case 'e':
			mCamera->move_ud(camera_frame_displacement);
			break;
		case 'k':
			two_viewport_mode = !two_viewport_mode;
			break;;
		case 'p': {
			if (mCamera->is_orthographic()) {
				mCamera->set_perspective();
			} else {
				mCamera->set_orthographic();
			}
			break;
		}
		default:
			if (key >= '0' && key <= '9' && !changeScene(key - '0'))
				cout << "[NOTE] There is no scene " << char(key) << ".\n";
			else
				need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	// Only interested in press events
	if (action == GLFW_RELEASE)
		return;

	bool need_redisplay = true;

	// Handle keyboard input
	// (key reference: https://www.glfw.org/docs/3.4/group__keys.html)
	switch (key) {
		case GLFW_KEY_ESCAPE:                     // Escape key
			glfwSetWindowShouldClose(mWindow, true); // stops main loop
			break;
		case GLFW_KEY_RIGHT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(ROTATION_SPEED); // rotates 1 on the X axis
			else
				mCamera->yawReal(-ROTATION_SPEED); // rotates -1 on the X axis
			break;
		case GLFW_KEY_LEFT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(-ROTATION_SPEED); // rotate -1 on the Y axis
			else
				mCamera->yawReal(ROTATION_SPEED); // rotates 1 on the Y axis
			break;
		case GLFW_KEY_UP:
			mCamera->pitchReal(ROTATION_SPEED); // rotates 1 on the Z axis
			break;
		case GLFW_KEY_DOWN:
			mCamera->pitchReal(-ROTATION_SPEED); // rotates -1 on the Z axis
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

bool
IG1App::changeScene(size_t sceneNr)
{
	// Check whether the scene exists
	if (sceneNr >= mScenes.size())
		return false;

	// Change only if a different scene
	if (sceneNr != mCurrentScene) {
		auto &&old_scene = *mScenes[mCurrentScene];
		// old_scene.unload();
		old_scene.destroy();

		mCurrentScene = sceneNr;
		auto &&new_scene = *mScenes[mCurrentScene];
		new_scene.init();
		mNeedsRedisplay = true;
		// mScenes[mCurrentScene]->load();
	}

	return true;
}
