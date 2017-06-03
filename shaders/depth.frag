#version 330

out float depth;
void main(void) {
	depth = gl_FragCoord.z;
}