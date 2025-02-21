#version 330 core

in vec3 debug_color;
in vec3 model_position;

uniform vec4 color;  // uniform color for all vertices
out vec4 FragColor;  // fixed-name color variable used by OpenGL

const vec3 extreme_colors[6] = vec3[6](
	vec3(1.0, 0.0, 0.0), // +X
	vec3(0.0, 1.0, 0.0), // +Y
	vec3(0.0, 0.0, 1.0), // +Z
	vec3(1.0, 1.0, 0.0), // -X
	vec3(0.0, 1.0, 1.0), // -Y
	vec3(1.0, 0.0, 1.0) // -Z
);

void main()
{
	// extreme color is choses according to the greatest axis of the model position
	vec3 extreme_color = vec3(1.0);
	if (abs(model_position.x) > abs(model_position.y) && abs(model_position.x) > abs(model_position.z))
	{
		if (model_position.x > 0.0)
			extreme_color = extreme_colors[0];
		else
			extreme_color = extreme_colors[3];
	} else if (abs(model_position.y) > abs(model_position.z))
	{
		if (model_position.y > 0.0)
			extreme_color = extreme_colors[1];
		else
			extreme_color = extreme_colors[4];
	} else {
		if (model_position.z > 0.0)
			extreme_color = extreme_colors[2];
		else
			extreme_color = extreme_colors[5];
	}
	
	vec3 out_color = vec3(1.0) - ((vec3(1.0) - color.rgb) * (vec3(1.0) - extreme_color));
	// FragColor = vec4(out_color * debug_color, 1.0);
	FragColor = vec4(debug_color, 1.0);
}
