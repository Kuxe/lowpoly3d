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

vec3 suneffects() {

	//Increase radial lightning around sun exponentially fast as angle between
	//view vector and sun vector (in world space) approach 0
	//should move sun to screenspace.. it is already in screenspace.. rite..
	return dot(vec3(0.0, 0.0, 1.0), vec3(projection*view*normalize(vec4(sunPos, 1.0)))) * timeOfDayColor;
}

void main(void) {

	float d = distance(vec2(gl_FragCoord), windowResolution/2.0) / max(windowResolution.x, windowResolution.y);
	float filterStrength = 1 - (0.5 / (1 + exp(-d)));
	vec3 ambient = vec3(0.15, 0.15, 0.15);
	float diffuse = max(dot(geomOutNormal, normalize(sunPos - vec3(screen2world()))), 0.1) * sigmoid(sunPos.y, 1);
	color = (diffuse * geomOutColor + geomOutColor * ambient) * normalize(timeOfDayColor) + 0.0001*suneffects();
	//Modulate color as a function of time of day (orange during sunrise / sunset and dark at night!)
}