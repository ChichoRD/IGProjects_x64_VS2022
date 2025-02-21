#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 3) in vec3 aNormal;  // input vertex in local coordinates

uniform mat4 model;  // model matrix
uniform mat4 view; // view matrix
uniform mat4 view_projection; // projection * view matrix

out vec3 FragPos;  // for the fragment shader
out vec3 Normal;

void main()
{
	gl_Position = view_projection * (model * vec4(aPos, 1.0));

	const mat4 model_view = view * model;
	FragPos = vec3(model_view * vec4(aPos, 1.0));
	Normal = (model_view * vec4(aNormal, 0.0)).xyz; // we assume uniform scale
}
