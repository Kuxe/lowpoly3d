#version 450

out vec3 color;

uniform sampler2D renderedTexture;
uniform vec2 resolution;

vec3 sobel() {
	return -1.0 / 16.0*(
		2.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(1, 0))/resolution).xyz +
		2.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 0))/resolution).xyz +
		2.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, 1))/resolution).xyz +
		2.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, -1))/resolution).xyz +
		texture2D(renderedTexture, (gl_FragCoord.xy + vec2(1, 1))/resolution).xyz +
		texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, -1))/resolution).xyz -
		12*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, 0))/resolution).xyz)
	;
}

float edge() {
	return dot(sobel(), vec3(1.0)) / 3.0;
}

vec3 sumNeighbours(int n) {
	vec3 sum = vec3(0.0);
	for(int x = 0; x < n; x++) {
		for(int y = 0; y < n; y++) {
			sum += texture(renderedTexture, (gl_FragCoord.xy + vec2(x, y))/resolution).xyz;
		}
	}
	return sum;
}

vec3 avgNeighborus(int n) {
	return sumNeighbours(n) / pow(n, 2.0);
}

vec3 screenspaceAA() {
	return (
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(1, 0))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 0))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, 1))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, -1))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(1, 1))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, 1))/resolution).xyz +
		1.0/18.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(-1, -1))/resolution).xyz +
		1.0/2.0*texture2D(renderedTexture, (gl_FragCoord.xy + vec2(0, 0))/resolution).xyz)
	;
}

//TODO: Toy around with post-processing effects
void main(void) {
	color = texture(renderedTexture, gl_FragCoord.xy/resolution).xyz;
	color = 0.6*avgNeighborus(5) * color + 0.6*pow(color, vec3(2.0));
}
