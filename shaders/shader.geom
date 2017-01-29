#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex
{
  vec3 color;
  vec4 fragsunSpace;
} vertex[];

out vec3 geomOutColor;
out vec3 geomOutNormal;
out vec4 geomOutFragSunSpace;

layout (std140) uniform WorldUniformData { 
	mat4 view, projection;
	vec4 sunPos, timeOfDayColor;
	vec2 windowResolution;
};

void main() {
	mat4 vpi = inverse(projection*view);
	//Since I stated that I output triangle_strip of 3 vertices, I need to define all these (emit them 3 times)
	for(int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		geomOutColor = vertex[i].color;
		geomOutFragSunSpace = vertex[i].fragsunSpace;

		vec4 frag0world = vpi * gl_in[0].gl_Position;
		vec4 frag1world = vpi * gl_in[1].gl_Position;
		vec4 frag2world = vpi * gl_in[2].gl_Position;
		geomOutNormal = normalize(cross(vec3(frag0world - frag1world), vec3(frag0world - frag2world)));
		EmitVertex();
	}
	EndPrimitive();
}