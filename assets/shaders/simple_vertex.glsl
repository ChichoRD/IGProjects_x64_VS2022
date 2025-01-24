#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates

uniform mat4 modelView;  // model-view matrix
uniform mat4 projection; // projection

void main()
{
	//gl_Position = vec4(aPos - projection[0][0], 1.0);
	gl_Position = projection * (modelView * vec4(aPos, 1.0));
	//gl_Position = vec4(aPos + modelView[3][3] * 0.5, 1.0);
	//gl_Position = vec4(aPos.xyz, 1.0) * modelView;
}
