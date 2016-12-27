#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;


out Vertex {
	vec3 color;
} vertex;

uniform mat4 mvp;


void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	vertex.color = color;
}