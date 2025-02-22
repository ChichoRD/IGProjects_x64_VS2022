#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 1) in vec4 aColor;  // input vertex in local coordinates
layout (location = 2) in vec2 aTexCoord;  // texture coordinates

out vec3 sampled_displacement;
out vec3 position_world;

uniform mat4 model;  // model matrix
uniform mat4 view_projection; // projection * view matrix

uniform sampler2D displacement_map;
uniform float displacement_scale;
uniform float displacement_factor;

void main()
{
    vec3 sampled = texture(displacement_map, vec2(aTexCoord.x, 1.0 - aTexCoord.y)).xyz;
    vec3 displacement = sampled * 2.0 - vec3(1.0);

    if (aPos.z < 0.0) {
        displacement.z = -displacement.z;
    }

    vec3 displaced_position = aPos + displacement * displacement_scale;
    vec3 position = mix(aPos, displaced_position, displacement_factor);

    position_world = (model * vec4(position, 1.0)).xyz;
	gl_Position = view_projection * vec4(position_world, 1.0);

    sampled_displacement = sampled;
}
