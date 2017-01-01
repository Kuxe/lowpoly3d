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

vec3 sumNeighbours(int n) {
	vec3 sum = vec3(0.0);
	for(int x = 0; x < n; x++) {
		for(int y = 0; y < n; y++) {
			sum += texture2D(renderedTexture, (gl_FragCoord.xy + vec2(x, y))/resolution).xyz;
		}
	}
	return sum;
}

vec3 avgNeighborus(int n) {
	return sumNeighbours(n) / pow(n, 2);
}

//TODO: Toy around with post-processing effects
void main(void) {
	vec3 thisColor = texture2D(renderedTexture, gl_FragCoord.xy/resolution).xyz; 
	color = thisColor;
}
