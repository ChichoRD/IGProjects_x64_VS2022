#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 1) in vec4 aColor;  // input vertex in local coordinates
layout (location = 2) in vec2 aTexCoord;  // texture coordinates

uniform mat4 model;  // model matrix
uniform mat4 view_projection; // projection * view matrix

sampler2D displacement_map;
uniform float displacement_scale;
uniform float displacement_factor;

void main()
{
    const vec3 displacement = texture(displacement_map, aTexCoord).xyz - vec3(0.5);
    const vec3 displaced_position = aPos + displacement * displacement_scale;

    const vec3 position = mix(aPos, displaced_position, displacement_factor);
	gl_Position = view_projection * (model * vec4(position.xyz, 1.0));
}
