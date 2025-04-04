#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Viewport.h"

class Camera
{
public:
	explicit Camera(const Viewport vp);
	~Camera() = default;


	// viewPort
	Viewport const& viewport() const { return mViewPort; };
	Viewport &viewport() { return mViewPort; };

	// view matrix
	glm::dmat4 const& viewMat() const { return mViewMat; };

	inline void set2D() { set_orthographic(); }
	inline void set3D() { set_perspective(); }

	void pitch(GLdouble a); // rotates a degrees on the X axis
	void yaw(GLdouble a);   // rotates a degrees on the Y axis
	void roll(GLdouble a);  // rotates a degrees on the Z axis

	void pitchReal(GLfloat cs);
	void yawReal(GLfloat cs);
	void rollReal(GLfloat cs);

	// projection matrix
	glm::dmat4 const& projMat() const { return mProjMat; };

	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const;

protected:
	glm::dvec3 mEye = {0.0, 0.0, 500.0}; // camera's position
	glm::dvec3 mLook = {0.0, 0.0, 0.0};  // target's position
	glm::dvec3 mUp = {0.0, 1.0, 0.0};    // the up vector

	glm::vec3 mRight, mUpward, mFront;
	GLdouble fov = glm::half_pi<float>();

	void setAxes();

	glm::dmat4 mViewMat;   // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::dmat4 mProjMat;   // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	void upload_view_projection() const;

	GLdouble xRight, xLeft, yTop, yBot;     // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000; // view volume
	GLdouble mScaleFact = 1;                // scale factor
	
	Viewport mViewPort; // the viewport
	bool bOrto = true;                      // orthogonal or perspective projection

	void setVM();
	void setPM();

public:
	inline glm::vec3 right() {
		return mRight;
	}
	inline glm::vec3 up() {
		return mUpward;
	}
	inline glm::vec3 front() {
		return mFront;
	}

	inline GLdouble scale() {
		return mScaleFact;
	}

	inline void move_lr(GLfloat displacement) {
		mEye += mRight * displacement;
		mLook += mRight * displacement;
		setVM();
	}

	inline void move_fb(GLfloat displacement) {
		mEye += mFront * displacement;
		mLook += mFront * displacement;
		setVM();
	}

	inline void move_ud(GLfloat displacement) {
		mEye += mUpward * displacement;
		mLook += mUpward * displacement;
		setVM();
	}

	glm::dvec3 orbit_xz(
		const GLfloat disaplacement_radians,
		const GLfloat displacement_altitude,
		const GLfloat focal_length
	);
	
	inline void set_orthographic() {
		bOrto = true;
		setPM();
	}
	inline void set_perspective() {
		bOrto = false;
		setPM();
	}
	inline bool is_orthographic() const { return bOrto; }

	inline void set_position(const glm::dvec3 eye_position) {
		mEye = eye_position;
		mLook = mEye + glm::dvec3{mFront};
		setVM();
	}

	inline void look_at(const glm::dvec3 look_position, const glm::dvec3 up_vector) {
		mLook = look_position;
		mUp = up_vector;
		setVM();
	}
};

void camera_set_cenital(Camera &camera, const glm::dvec3 eye_position);
void camera_set_cenital_orthographic(Camera &camera, const glm::dvec3 eye_position);
void camera_set_cenital_perspective(Camera &camera, const glm::dvec3 eye_position);

#endif //_H_Camera_H_
