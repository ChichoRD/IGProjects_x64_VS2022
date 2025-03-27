#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace glm;

Camera::Camera(Viewport* vp)
  : mViewMat(1.0)
  , mProjMat(1.0)
  , xRight(vp->width() / 2.0)
  , xLeft(-xRight)
  , yTop(vp->height() / 2.0)
  , yBot(-yTop)
  , mViewPort(vp)
{
	setPM();
}

void
Camera::uploadVM() const
{
	Shader::setUniform4All("view", mViewMat);
}

void Camera::setAxes()
{
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = {0, 0, 500};
	mLook = {0, 0, 0};
	mUp = {0, 1, 0};
	setVM();
}

void
Camera::set3D()
{
	mEye = {500, 500, 500};
	mLook = {0, 10, 0};
	mUp = {0, 1, 0};
	setVM();
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::pitchReal(GLfloat cs)
{
	mViewMat = rotate(mViewMat, (GLdouble)glm::radians(cs), (glm::dvec3)mRight);
	setAxes();
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yawReal(GLfloat cs)
{
	mViewMat = rotate(mViewMat, (GLdouble)glm::radians(cs), (glm::dvec3)mUpward);
	setAxes();
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::rollReal(GLfloat cs)
{
	mViewMat = rotate(mViewMat, (GLdouble)glm::radians(cs), (glm::dvec3)mFront);
	setAxes();
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

#include <iostream>
void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	} else{
		std::cout << "scale factor: " << mScaleFact << std::endl;
		std::cout << "fov: " << fov << std::endl;
		std::cout << "fov / (2.0 - mScaleFact): " << fov / (2.0 - mScaleFact) << std::endl;
		mProjMat = perspective(fov / (2.0f - mScaleFact), xRight/yTop, mNearVal, mFarVal);
	}
}

glm::dvec3 Camera::orbit_xz(const GLfloat disaplacement_radians, const GLfloat displacement_altitude) {
    const GLfloat rotation_xz_cos_sin[2]{
		glm::cos(disaplacement_radians), glm::sin(disaplacement_radians)
	};

	const glm::dvec3 eye_from_look = mEye - mLook;
	const glm::dvec3 eye_from_look_xy_rotated{
		eye_from_look.x * rotation_xz_cos_sin[0] - eye_from_look.z * rotation_xz_cos_sin[1],
		eye_from_look.y + displacement_altitude,
		eye_from_look.x * rotation_xz_cos_sin[1] + eye_from_look.z * rotation_xz_cos_sin[0]
	};

	const glm::vec3 eye_rotated = mLook + eye_from_look_xy_rotated;
	mEye = eye_rotated;

	setVM();
	return eye_rotated;
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void Camera::upload_view_projection() const {
	Shader::setUniform4All("view_projection", mProjMat * mViewMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	// uploadPM();
	upload_view_projection();
}
