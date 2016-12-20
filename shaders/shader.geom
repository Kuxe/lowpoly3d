#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex
{
  vec3 color;
} vertex[];

out vec3 geomOutColor;
out vec3 geomOutNormal;

uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 vpi = inverse(projection*view);
	//Since I stated that I output triangle_strip of 3 vertices, I need to define all these (emit them 3 times)
	for(int i = 0; i < 3; i++) {
		gl_Position = gl_in[i].gl_Position;
		geomOutColor = vertex[i].color;

		vec4 frag0world = vpi * gl_in[0].gl_Position;
		vec4 frag1world = vpi * gl_in[1].gl_Position;
		vec4 frag2world = vpi * gl_in[2].gl_Position;
		geomOutNormal = normalize(cross(vec3(frag0world - frag1world), vec3(frag0world - frag2world)));
		EmitVertex();
	}
	EndPrimitive();
}