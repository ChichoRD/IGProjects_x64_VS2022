#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "Scene.h"

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

	const glm::vec4 light_direction_view{
		glm::normalize(mViewMat * glm::dvec4{ -1.0, -1.0, -1.0, 0.0 })
	};
	Shader& simple_light{ *Shader::get("simple_light") };
	simple_light.use();
	simple_light.setUniform(
		"lightDir",
		light_direction_view
	);
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

glm::vec3 Camera::right() const {
	return mRight;
}

glm::vec3 Camera::up() const {
	return mUpward;
}

glm::vec3 Camera::front() const {
	return mFront;
}

GLdouble Camera::scale() const {
	return mScaleFact;
}

void Camera::move_lr(GLfloat displacement) {
	mEye += mRight * displacement;
	mLook += mRight * displacement;
	setVM();
}

void Camera::move_fb(GLfloat displacement) {
	mEye += mFront * displacement;
	mLook += mFront * displacement;
	setVM();
}

void Camera::move_ud(GLfloat displacement) {
	mEye += mUpward * displacement;
	mLook += mUpward * displacement;
	setVM();
}

glm::dvec3 Camera::orbit_xz(
	const GLfloat disaplacement_radians,
	const GLfloat displacement_altitude,
	const GLfloat focal_length
) {
	// (void)focal_length;
	mEye = glm::rotate(
		glm::identity<dmat4>(),
		double{disaplacement_radians},
		glm::dvec3{mUpward}
	) * glm::dvec4{mEye, 0.0};
	mLook = glm::dvec3{ 0.0 };

	mEye.y += displacement_altitude * 10.0;
	setVM();
	return mEye;
}

void Camera::set_orthographic() {
	bOrto = true;
	setPM();
}

void Camera::set_perspective() {
	bOrto = false;
	setPM();
}

void Camera::set_position(const glm::dvec3 eye_position) {
	mEye = eye_position;
	mLook = mEye + glm::dvec3{ mFront };
	setVM();
}

void Camera::look_at(const glm::dvec3 look_position, const glm::dvec3 up_vector) {
	mLook = look_position;
	mUp = up_vector;
	setVM();
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

void Camera::right_view() {
	mEye = glm::dvec3(Scene::axis_unit_size, 0, 0);
	mLook = glm::dvec3(0,0,0);
	mUp = glm::dvec3(0,1,0);
	setVM();
}

void Camera::front_view() {
	mEye = glm::dvec3(0, 0, Scene::axis_unit_size);
	mLook = glm::dvec3(0, 0, 0);
	mUp = glm::dvec3(0, 1, 0);
	setVM();
}

void Camera::top_view() {
	mEye = glm::dvec3(0, Scene::axis_unit_size, 0);
	mLook = glm::dvec3(0, 0, 0);
	mUp = glm::dvec3(0, 0, -1);
	setVM();
}

void Camera::isometric_view() {
	mEye = glm::dvec3(Scene::axis_unit_size, Scene::axis_unit_size, Scene::axis_unit_size);
	mLook = glm::dvec3(0, 0, 0);
	mUp = glm::dvec3(0.0f, 1.0f, 0.0f);
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

