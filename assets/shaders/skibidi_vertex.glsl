#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 1) in vec4 aColor;  // input vertex in local coordinates
layout (location = 2) in vec2 aTexCoord;  // texture coordinates

out vec3 debug_color;
out vec3 model_position;

uniform mat4 model;  // model matrix
uniform mat4 view_projection; // projection * view matrix

uniform sampler2D displacement_map;
uniform float displacement_scale;
uniform float displacement_factor;

void main()
{
    vec3 displacement = texture(displacement_map, aTexCoord).xyz * 2.0 - vec3(1.0);
    displacement = step(0.1, dot(displacement, displacement)) * displacement;
    vec3 displaced_position = aPos + displacement * displacement_scale;

    vec3 position = mix(aPos, displaced_position, displacement_factor);
	gl_Position = view_projection * (model * vec4(position.xyz, 1.0));

    debug_color = displacement * 0.5 + vec3(0.5);
    model_position = position;
    // debug_color = position;
    // debug_color = vec3(displacement_factor);
}
