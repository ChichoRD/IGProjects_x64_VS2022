#ifndef _H_Light_H
#define _H_Light_H

#include <string>

#include "Shader.h"
#include <glm/glm.hpp>
#include <cmath>

class Light { // Abstract class
public:
	virtual ~Light();

	bool enabled() const { return bEnabled; }
	void setEnabled(bool enabled) { bEnabled = enabled; }

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const;

	void unload(Shader& shader);

	void setAmb(const glm::vec3& ind);
	void setDiff(const glm::vec3& ind);
	void setSpec(const glm::vec3& ind);

protected:
	Light(std::string name);
	Light(const std::string& name, int id);

	// Identifier
	std::string lightID;
	bool bEnabled;

	// Atributos lumínicos y geométrico de la fuente de luz
	glm::vec3 ambient = {0.1, 0.1, 0.1};
	glm::vec3 diffuse = {0.5, 0.5, 0.5};
	glm::vec3 specular = {0.5, 0.5, 0.5};
};

class DirLight : public Light {
public:
	explicit DirLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir);

protected:
	glm::vec4 direction = {-1, -1, -1, 0};
};

inline void
DirLight::setDirection(const glm::vec3& dir) {
	assert(
		(std::fabs(glm::dot(dir, dir) - 1.0f) < 0.0001f)
		&& "fatal error: direction vector must be normalized"
	);
	direction = glm::vec4(dir, 0.0);
}

class PosLight : public Light {
public:
	explicit PosLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;

	inline void setPosition(const glm::fvec3& dir) {
		position = glm::vec4(dir, 1.0);
	}
	inline glm::vec3 get_position() const {
		return glm::vec3(position);
	}
	inline void setAttenuation(GLfloat kc, GLfloat kl, GLfloat kq) {
		constant = kc;
		linear = kl;
		quadratic = kq;
	}

protected:
	glm::vec4 position = {0, 0, 0, 1};
	// Factores de atenuación
	GLfloat constant = 1, linear = 0, quadratic = 0;
};

class SpotLight : public PosLight {
public:
	SpotLight(const glm::vec3& pos = {0, 0, 0}, int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir) { direction = dir; }
	void setCutoff(float inner, float outer);

protected:
	// Atributos del foco
	glm::vec3 direction = {0, 0, -1};
	GLfloat cutoff = 60, outerCutoff = 120;
};

#endif //_H_Light_H_
