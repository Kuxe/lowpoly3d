#version 330

out vec3 color;
in vec3 fragColor;

//TODO: Implement some procedural solar flares (simplex noise perhaps?)
void main(void) {
	color = fragColor + vec3(1.0);
}