#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
uniform vec3 sunPosition;
uniform mat4 sunmvp;
uniform mat4 mvp;

out Vertex {
	vec3 color;
	vec4 fragsunSpace;
} vertex;

//Simple pass-through vertex shader
void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	vertex.color = color;
	vertex.fragsunSpace = sunmvp * vec4(position, 1.0);
}