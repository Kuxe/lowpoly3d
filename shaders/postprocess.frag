#version 450

out vec3 color;

uniform sampler2D renderedTexture;
uniform vec2 resolution;

//TODO: Toy around with post-processing effects
void main(void) {
	color = texture2D(renderedTexture, gl_FragCoord.xy/resolution).xyz;
}