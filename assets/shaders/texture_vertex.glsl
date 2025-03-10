#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 1) in vec4 aColor;  // input vertex in local coordinates
layout (location = 2) in vec2 aTexCoord;  // texture coordinates

uniform mat4 model;  // model matrix
uniform mat4 view_projection; // projection * view matrix

out vec4 vertexColor;
out vec2 texCoord;

void main()
{
	gl_Position = view_projection * (model * vec4(aPos.x, aPos.y, aPos.z, 1.0));
	vertexColor = aColor;
	texCoord = aTexCoord;
}
