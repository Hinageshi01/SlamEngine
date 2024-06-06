#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

layout(location = 0) out vec2 v_uv;

uniform mat4 u_modelViewProjection;

void main()
{
	v_uv = a_uv;
	gl_Position = u_modelViewProjection * vec4(a_position, 1.0);
}
