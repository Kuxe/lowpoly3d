#version 450

in vec3 geomOutColor;
in vec3 geomOutNormal;
out vec3 color;

uniform vec3 sunPos;
uniform vec3 timeOfDayColor;
uniform vec2 windowResolution;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 sunvp;

uniform sampler2D shadowmap;

vec4 screen2world() {
	vec2 eyeCoords = mat2(inverse(projection)) * 2*(vec2(gl_FragCoord.x / windowResolution.x, gl_FragCoord.y / windowResolution.y) - vec2(0.5));
	return inverse(view) * vec4(eyeCoords.x, eyeCoords.y, -gl_FragCoord.z, gl_FragCoord.w) / gl_FragCoord.w;
}

float sigmoid(float x, float k) {
	return 1.0/(1.0 + k*exp(-x));
}

//Returns a floating point in range [0.0, 1.0] representing "how much" this fragment is shadowed
float shadowed(vec4 fragworld) {
	//Lookup value at shadowmap in sun-space, transform this fragment to sun-space
	//and comparse depth of this fragment with depth of shadowmap **/
	vec3 shadowed = texture2D(shadowmap, gl_FragCoord.xy / windowResolution).xyz;
	float shadow = shadowed.z;
	fragworld.xy *= 2.0*windowResolution;
	vec4 fragsun = sunvp * fragworld;
	return float(fragsun.z < shadow);
}

void main(void) {
	vec4 fragworld = screen2world();
	vec3 ambient = vec3(0.15, 0.15, 0.15);
	float diffuse = max(dot(geomOutNormal, normalize(sunPos - vec3(fragworld))), 0.1) * sigmoid(sunPos.y, 1);
	color = mix((diffuse * geomOutColor + geomOutColor * ambient), timeOfDayColor, 0.05) * shadowed(fragworld);
}