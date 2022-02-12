#version 330

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

void main(void) {
	color = geomOutColor;
}