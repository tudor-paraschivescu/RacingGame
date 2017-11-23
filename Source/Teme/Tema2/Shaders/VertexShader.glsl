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
uniform float hour;
uniform int id;

// IDs that help identify the current shape
uniform int ROADPART_ID = 0;
uniform int EARTH_ID = 1;
uniform int SKY_ID = 2;

// Colors
uniform vec3 EARTH_DAY_COLOR = { 0.1, 0.75, 0.1 };
uniform float EARTH_TRANSITION_GREEN = 0.5;
uniform vec3 ROADPART_DAY_COLOR = { 0.5, 0.5, 0.5 };
uniform vec3 ROADPART_TRANSITION_ALL = { 0.25, 0.25, 0.25 };
uniform vec3 SKY_DAY_COLOR = { 0.1, 0.1, 0.75 };
uniform float SKY_TRANSITION_BLUE = 0.5;

// Time Values
uniform float DAY_START_HOUR = 7.0;
uniform float NIGHT_START_HOUR = 20.0;
uniform float TRANSITION_TIME = 2.0;

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

	if (hour < DAY_START_HOUR || hour > NIGHT_START_HOUR + TRANSITION_TIME) {
		// Night Time
		if (id == EARTH_ID) {
			frag_vertex_color = EARTH_DAY_COLOR - vec3(0, EARTH_TRANSITION_GREEN, 0);
		}
		else if (id == ROADPART_ID) {
			frag_vertex_color = ROADPART_DAY_COLOR - ROADPART_TRANSITION_ALL;
		}
		else if (id == SKY_ID) {
			frag_vertex_color = SKY_DAY_COLOR - vec3(0, 0, SKY_TRANSITION_BLUE);
		}
	}
	else if (hour < DAY_START_HOUR + TRANSITION_TIME) {
		// Dawn Time
		float change = (hour - DAY_START_HOUR) / TRANSITION_TIME;
		if (id == EARTH_ID) {
			frag_vertex_color = EARTH_DAY_COLOR - vec3(0, (1 - change) * EARTH_TRANSITION_GREEN, 0);
		}
		else if (id == ROADPART_ID) {
			frag_vertex_color = ROADPART_DAY_COLOR - ROADPART_TRANSITION_ALL * (1.0 - change);
		}
		else if (id == SKY_ID) {
			frag_vertex_color = SKY_DAY_COLOR - vec3(0, 0, (1 - change) * SKY_TRANSITION_BLUE);
		}
	}
	else if (hour < NIGHT_START_HOUR) {
		// Day Time
		if (id == EARTH_ID) {
			frag_vertex_color = EARTH_DAY_COLOR;
		}
		else if (id == ROADPART_ID) {
			frag_vertex_color = ROADPART_DAY_COLOR;
		}
		else if (id == SKY_ID) {
			frag_vertex_color = SKY_DAY_COLOR;
		}
	} 
	else {
		// Sunset Time
		float change = (hour - NIGHT_START_HOUR) / TRANSITION_TIME;
		if (id == EARTH_ID) {
			frag_vertex_color = EARTH_DAY_COLOR - vec3(0, change * EARTH_TRANSITION_GREEN, 0);
		}
		else if (id == ROADPART_ID) {
			frag_vertex_color = ROADPART_DAY_COLOR - ROADPART_TRANSITION_ALL * change;
		}
		else if (id == SKY_ID) {
			frag_vertex_color = SKY_DAY_COLOR - vec3(0, 0, change * SKY_TRANSITION_BLUE);
		}
	}

	// Compute gl_Position
	gl_Position = Projection * View * Model * vec4(vertex_position, 1.0);
}
