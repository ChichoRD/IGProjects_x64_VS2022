#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

using namespace glm;

Camera::Camera(const Viewport vp)
  : mViewMat(1.0)
  , mProjMat(1.0)
  , xRight(vp.width() / 2.0)
  , xLeft(-xRight)
  , yTop(vp.height() / 2.0)
  , yBot(-yTop)
  , mViewPort(vp)
  ,	mEye{0.0, 0.0, 0.0}
  , mLook{0.0, 0.0, 1.0}
  , mUp{0.0, 1.0, 0.0}
  , bOrto(false)
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
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::pitchReal(GLfloat cs) {
	glm::dvec3 look_from_eye = mLook - mEye;
	glm::dvec3 rotated_look_from_eye =
		glm::rotate(glm::identity<dmat4>(), (GLdouble)cs, (glm::dvec3)mRight)
		* dvec4{look_from_eye, 0.0};
	// mViewMat = rotate(mViewMat, (GLdouble)glm::radians(cs), (glm::dvec3)mRight);

	glm::dvec3 rotated_up = glm::cross(dvec3{mRight}, rotated_look_from_eye);

	mUp = glm::normalize(rotated_up);
	mLook = mEye + rotated_look_from_eye;
	setVM();
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, a, glm::dvec3(0, 1.0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yawReal(GLfloat cs)
{
	glm::dvec3 look_from_eye = mLook - mEye;
	glm::dvec3 rotated_look_from_eye =
		glm::rotate(glm::identity<dmat4>(), (GLdouble)cs, (glm::dvec3)mUpward)
		* dvec4{look_from_eye, 0.0};
	// mViewMat = rotate(mViewMat, (GLdouble)glm::radians(cs), (glm::dvec3)mUpward);

	mLook = mEye + rotated_look_from_eye;
	setVM();
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, a, glm::dvec3(0, 0, 1.0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::rollReal(GLfloat cs)
{
	//rolls real
	mUpward = glm::rotate(glm::identity<dmat4>(), (GLdouble)cs, (glm::dvec3)mFront) * dvec4{mUpward, 0.0};
	mUp = mUpward;
	setVM();
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
		// std::cout << "scale factor: " << mScaleFact << std::endl;
		// std::cout << "fov: " << fov << std::endl;
		// std::cout << "fov / (2.0 - mScaleFact): " << fov / (2.0 - mScaleFact) << std::endl;
		mProjMat = perspective(fov / (2.0f - mScaleFact), xRight/yTop, mNearVal, mFarVal);
	}
}

glm::dvec3 Camera::orbit_xz(
	const GLfloat disaplacement_radians,
	const GLfloat displacement_altitude,
	const GLfloat focal_length
) {
    const GLfloat rotation_xz_cos_sin[2]{
		glm::cos(disaplacement_radians), glm::sin(disaplacement_radians)
	};
 
	const glm::dvec3 look_from_eye_normalized = glm::normalize(mLook - mEye);
	const glm::dvec3 focus_displacement = look_from_eye_normalized * double(focal_length);
	const glm::dvec3 focus = mEye + focus_displacement;

	const glm::dvec3 eye_from_look = focus - mLook; //skibdi rizz
	const glm::dvec3 eye_from_look_xy_rotated{
		eye_from_look.x * rotation_xz_cos_sin[0] - eye_from_look.z * rotation_xz_cos_sin[1],
		eye_from_look.y + displacement_altitude,
		eye_from_look.x * rotation_xz_cos_sin[1] + eye_from_look.z * rotation_xz_cos_sin[0]
	};

	const glm::vec3 eye_rotated = mLook + eye_from_look_xy_rotated / double(focal_length);
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
	mViewPort.upload();
	uploadVM();
	// uploadPM();
	upload_view_projection();
}

void camera_set_cenital(Camera &camera, const glm::dvec3 eye_position) {
	camera.set_position(eye_position);

	glm::dvec3 new_look = glm::dvec3{eye_position.x, 0.0, eye_position.z};
	glm::dvec3 new_look_from_eye{new_look - eye_position};

	glm::dvec3 new_up = glm::cross(glm::dvec3{camera.right()}, new_look_from_eye);
	camera.look_at(new_look, new_up);
}

void camera_set_cenital_orthographic(Camera &camera, const glm::dvec3 eye_position) {
    camera.set_orthographic();
	camera_set_cenital(camera, eye_position);
	//camera.setSize(eye_position.x * 2.0, eye_position.z * 2.0);
	//camera.setScale(1.0);
}

void camera_set_cenital_perspective(Camera &camera, const glm::dvec3 eye_position) {
	camera.set_perspective();
	camera_set_cenital(camera, eye_position);
	//camera.setSize(eye_position.x * 2.0, eye_position.z * 2.0);
	//camera.setScale(1.0);
}
