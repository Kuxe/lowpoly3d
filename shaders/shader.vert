#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
uniform vec3 sunPosition;
uniform mat4 sunvp;

out Vertex {
	vec3 color;
	vec4 fragsunSpace;
} vertex;

uniform mat4 mvp;

//Simple pass-through vertex shader
void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	vertex.color = color;
	vertex.fragsunSpace = sunvp * vec4(position, 1.0);
}