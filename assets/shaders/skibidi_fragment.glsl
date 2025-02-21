#version 330 core

in vec3 debug_color;

uniform vec4 color;  // uniform color for all vertices
out vec4 FragColor;  // fixed-name color variable used by OpenGL

void main()
{
	// FragColor = vec4(debug_color, 1.0);
	FragColor = color * vec4(debug_color, 1.0);
}
