#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;

layout (std140) uniform WorldUniformData { 
	mat4 view, projection;
	vec4 sunPos, timeOfDayColor;
	vec2 windowResolution;
};

layout (std140) uniform ModelUniformData { 
	mat4 model, mvp, sunmvp;
};

void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	fragColor = vec3(timeOfDayColor);
}