#version 450

out vec3 color;
in vec3 fragColor;

layout (std140) uniform WorldUniformData {
	mat4 view, projection;
	vec4 sunPos, timeOfDayColor;
	vec2 windowResolution;
};

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

vec3 suneffects(float sunIntensity) {
	float fragsunDistance = distance(world2screen(vec3(sunPos)), gl_FragCoord.xy);
	vec3 sunFlareColor = vec3(1.0, 0.2, 0.0);
	//lookatLevel can be though of as "how much am I lookin at sun", ranging from 0.0 to 1.0, non-linear function.
	//it also helps eliminate a "night sun" which appear due to reasons I wont bother explaining because I dont
	//fully understand them atm.. I did grasp it recently but details are forgotten..
	float lookatLevel = max(0.0, dot(vec3(0.0, 0.0, -1.0), normalize(vec3(view*sunPos))));
	return sunIntensity/fragsunDistance * sunFlareColor * lookatLevel;
}

void main(void) {
	color = fragColor + suneffects(30.0);
}