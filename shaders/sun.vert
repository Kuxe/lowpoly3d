#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;

uniform mat4 mvp;

void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	fragColor = color;
}