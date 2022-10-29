#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 3) in vec3 v_normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float timeElapsed;

out vec3 frag_color;
out vec3 frag_normal;

void main(){
	frag_normal = v_normal;
	frag_color  = v_color;

	vec3 pos = v_position;

	

	gl_Position = Projection * View * Model * vec4(pos.x, pos.y, pos.z, 1.0);
}
