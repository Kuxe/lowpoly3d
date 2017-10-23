#version 330

out vec3 color;

uniform sampler2D renderedTexture;
uniform vec2 resolution;
uniform uint time;

//Random noise found on stackoverflow in range [0, 1]
float rand01(vec2 seed) {
    return fract(sin(dot(seed, vec2(12.9898,78.233))) * 43758.5453);
}

//Randon noise in range [-1, 1]
float unitrand(vec2 seed) {
	return rand01(seed)*2.0f - 1.0f;
}

vec3 sobel() {
	return -1.0 / 16.0*(
		2.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(1, 0))/resolution).xyz +
		2.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 0))/resolution).xyz +
		2.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, 1))/resolution).xyz +
		2.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, -1))/resolution).xyz +
		texture(renderedTexture, (gl_FragCoord.xy + vec2(1, 1))/resolution).xyz +
		texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, -1))/resolution).xyz -
		12*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, 0))/resolution).xyz)
	;
}

float edge() {
	return dot(sobel(), vec3(1.0)) / 3.0;
}

vec3 screenspaceAA() {
	return (
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(1, 0))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 0))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, 1))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, -1))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(1, 1))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		1.0/18.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(-1, -1))/resolution).xyz +
		1.0/2.0*texture(renderedTexture, (gl_FragCoord.xy + vec2(0, 0))/resolution).xyz)
	;
}

vec3 filmgrain(float strength, float seed) {
	return strength * vec3(unitrand(sin(time) * gl_FragCoord.xy), unitrand(sin(time+100u) * gl_FragCoord.xy), unitrand(sin(time+200u) * gl_FragCoord.xy));
}

//TODO: Toy around with post-processing effects
void main(void) {
	color = texture(renderedTexture, gl_FragCoord.xy/resolution).xyz;
	color = 0.7*color + pow(color, vec3(5.0));
	color = color + filmgrain(0.05f, time);


}
