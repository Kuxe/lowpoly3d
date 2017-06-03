#version 330

layout(location = 0) in vec3 position;

layout (std140) uniform ModelUniformData { 
	mat4 model, mvp, sunmvp;
};

void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
}