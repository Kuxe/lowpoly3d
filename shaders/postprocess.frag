#version 330

out vec3 color;

uniform sampler2D renderedTexture;
uniform vec2 resolution;

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

//TODO: Toy around with post-processing effects
void main(void) {
	color = texture(renderedTexture, gl_FragCoord.xy/resolution).xyz;
	color = 0.7*color + pow(color, vec3(5.0));
}
