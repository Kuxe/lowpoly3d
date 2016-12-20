#version 450

out vec3 color;
in vec3 fragColor;

void main(void) {
	color = fragColor;
}