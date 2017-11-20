#version 330

// Get values from fragment shader
in vec3 frag_vertex_position;
in vec3 frag_vertex_normal;
in vec3 frag_texture_coordinate;
in vec3 frag_vertex_color;

layout(location = 0) out vec4 out_vertex_color;

void main()
{
	// Write pixel out color
	out_vertex_color = vec4(abs(frag_vertex_color), 1);
}