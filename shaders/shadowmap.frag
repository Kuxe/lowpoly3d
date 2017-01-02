#version 450

out vec3 depth;
void main(void) {
	depth = vec3(gl_FragCoord.z);
}