#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates

uniform mat4 model;  // model matrix
uniform mat4 view_projection; // projection * view matrix

void main()
{
	gl_Position = view_projection * (model * vec4(aPos, 1.0));
}
