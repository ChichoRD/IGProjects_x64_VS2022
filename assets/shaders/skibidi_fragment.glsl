#version 330 core

in vec3 sampled_displacement;
in vec3 position_world;

uniform vec4 color;  // uniform color for all vertices
out vec4 FragColor;  // fixed-name color variable used by OpenGL

void main()
{
	vec3 out_color = color.rgb * sampled_displacement;
	
	// use position_world to create checkerboard pattern each 0.5 units

	const int checker_size = 51;
	ivec3 cell = ivec3(position_world / checker_size);
	if (((cell.x + cell.y * checker_size + cell.z * checker_size * checker_size) & 1) == 0) {
		out_color *= 0.8;
	}

	const float contrast = 2.0;
	out_color = (out_color - 0.5) * contrast + 0.5;
	FragColor = vec4(out_color, color.a);
}
