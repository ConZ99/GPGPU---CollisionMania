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
	frag_color  = vec3(0.9, 0.9, 0.3);

	vec3 pos = v_position;

	float k = 3.14;
	pos.z += 0.1*sin(k*(pos.x - 2*timeElapsed));
	pos.x += 0.1*sin(k*(pos.z - 2*timeElapsed));
	pos.y += 0.1*cos(k*(pos.x - 2*timeElapsed));

	gl_Position = Projection * View * Model * vec4(pos.x, pos.y, pos.z, 1.0);
}
