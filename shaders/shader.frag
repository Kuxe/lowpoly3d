#version 410

in vec3 geomOutColor;
in vec3 geomOutNormal;
in vec4 geomOutFragSunSpace;
out vec3 color;

layout (std140) uniform WorldUniformData { 
	mat4 view, projection;
	vec4 sunPos, timeOfDayColor;
	vec2 windowResolution;
};

uniform sampler2D shadowmap;

//             Modelmatrix                Viewmatrix               Projectionmatrix         Homogenous division   Multiply by windows resolution (NDC in [-1, 1] cube but screenspace in 0 - range so need to +1.0 then / 2.0)
//Modelspace --------------> Worldspace --------------> Cameraspace --------------> Clipspace --------------> NDCspace --------------> Screenspace 
//gl_FragCoord in screenspace

vec4 screen2world() {
	vec2 eyeCoords = mat2(inverse(projection)) * 2.0 *(vec2(gl_FragCoord.x / windowResolution.x, gl_FragCoord.y / windowResolution.y) - vec2(0.5));
	return inverse(view) * vec4(eyeCoords.x, eyeCoords.y, -gl_FragCoord.z, gl_FragCoord.w) / gl_FragCoord.w;
}

float sigmoid(float x, float k ) {
	return 1.0/(1.0 + k*exp(-x));
}

float readShadowmap(vec4 fragSunspace) {
	return texture(shadowmap, fragSunspace.xy).z;
}

//Returns a floating point in range [0.0, 1.0] representing "how much" this fragment is shadowed
float shadowed(vec4 fragSunspace) {
	return readShadowmap(fragSunspace) < (fragSunspace.z - 0.005) || sunPos.y < 0.0  ? 0.7 : 1.0;  
}

vec3 fog(vec3 color) {
	vec3 fogcolor = mix(vec3(0.5), vec3(timeOfDayColor), 0.8);
	return mix(color, fogcolor, clamp(sqrt(0.02*gl_FragCoord.z / gl_FragCoord.w) - 1.0, 0.0, 1.0));
}

void main(void) {

	vec4 shadowcoord = geomOutFragSunSpace / geomOutFragSunSpace.w;
	shadowcoord = shadowcoord * 0.5 + 0.5;

	vec4 fragworld = screen2world();
	vec3 ambient = vec3(0.15, 0.15, 0.15);
	float diffuse = max(dot(geomOutNormal, normalize(vec3(sunPos) - vec3(fragworld))), 0.1) * sigmoid(sunPos.y, 1);
	color = mix((diffuse * geomOutColor + geomOutColor * ambient), vec3(timeOfDayColor), 0.05) * shadowed(shadowcoord);
	color = fog(color);
}