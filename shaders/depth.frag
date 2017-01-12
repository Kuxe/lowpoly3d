#version 450

out float depth;
void main(void) {
	depth = gl_FragCoord.z;
}