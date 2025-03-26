#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Viewport.h"

class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix
	glm::dmat4 const& viewMat() const { return mViewMat; };

	void set2D();
	void set3D();

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
	GLdouble fov;

	void setAxes();

	glm::dmat4 mViewMat;   // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::dmat4 mProjMat;   // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	void upload_view_projection() const;

	GLdouble xRight, xLeft, yTop, yBot;     // size of scene visible area
	GLdouble mNearVal = 1, mFarVal = 10000; // view volume
	GLdouble mScaleFact = 1;                // scale factor
	
	Viewport* mViewPort; // the viewport
	bool bOrto = true;                      // orthogonal or perspective projection

	void setVM();
	void setPM();

public:
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

	inline void set_orthographic() {
		bOrto = true;
		setPM();
	}
	inline void set_perspective() {
		bOrto = false;
		setPM();
	}
	inline bool is_orthographic() const { return bOrto; }
};

#endif //_H_Camera_H_
