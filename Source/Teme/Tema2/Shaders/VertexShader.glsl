#version 330

// Get vertex attributes from each location
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 texture_coordinate;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader
out vec3 frag_vertex_position;
out vec3 frag_vertex_normal;
out vec3 frag_texture_coordinate;
out vec3 frag_vertex_color;

void main()
{
	// Send output to fragment shader
	frag_vertex_position = vertex_position;
	frag_vertex_normal = vertex_normal;
	frag_texture_coordinate = texture_coordinate;
	frag_vertex_color = vec3(0.5, 0.5, 0.5);

	// Compute gl_Position
	gl_Position = Projection * View * Model * vec4(vertex_position, 1.0);
}
