#version 450

in vec3 geomOutColor;
in vec3 geomOutNormal;
out vec3 color;

uniform vec3 sunPos;
uniform vec3 timeOfDayColor;
uniform vec2 windowResolution;

uniform mat4 view;
uniform mat4 projection;

vec4 screen2world() {
	vec2 eyeCoords = mat2(inverse(projection)) * 2*(vec2(gl_FragCoord.x / windowResolution.x, gl_FragCoord.y / windowResolution.y) - vec2(0.5));
	return inverse(view) * vec4(eyeCoords.x, eyeCoords.y, -gl_FragCoord.z, gl_FragCoord.w) / gl_FragCoord.w;
}

float sigmoid(float x, float k) {
	return 1/(1 + k*exp(-x));
}

void main(void) {
	vec3 ambient = vec3(0.15, 0.15, 0.15);
	float diffuse = max(dot(geomOutNormal, normalize(sunPos - vec3(screen2world()))), 0.1) * sigmoid(sunPos.y, 1);
	color = (diffuse * geomOutColor + geomOutColor * ambient) + 0.00001* normalize(timeOfDayColor);
}