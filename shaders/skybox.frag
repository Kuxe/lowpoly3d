#version 450

out vec3 color;
in vec3 fragColor;

uniform vec3 sunPos;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 windowResolution;

vec2 world2screen(vec3 p) {
	//Transform point to clipspace (projection * view * p transforms from
	//model space to view space to projection space aka clipspace
	//divide by w to get normalize device coordinate (NDC) space aka cube space [-1 1]^3
	//finally from NDC to screenspace simply shift range form [-1 1]^3 to [0 1]^2 (omit 3rd dimension)
	//and multiply with window resolution! Viola, you can now compare this 2d point with gl_FragCoord
	vec4 clipp = projection * view * vec4(p, 1.0);
	vec4 ndcp = clipp/clipp.w;
	vec2 screenp = vec2(ndcp + 1.0)/2.0;
	return screenp * windowResolution;
}

vec3 suneffects() {
	return 100.0/distance(world2screen(sunPos), gl_FragCoord.xy) * vec3(1.0, 0.2, 0.0);
}

void main(void) {
	color = fragColor + suneffects();
}