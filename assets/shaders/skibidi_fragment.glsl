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


float sd_box( in vec2 p, in vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

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

	const float box_side = 0.1;
	float box = sd_box(debug_color.xz, vec2(box_side)) / box_side;

	vec3 i_discard;
	vec3 mod_color = modf(debug_color, i_discard);
	mod_color = smoothstep(0.25, 0.5, mod_color);
	FragColor = vec4((mod_color * min(mix(1.0, 0.0, 1.0 - box), 1.0)) , 1.0);
}
