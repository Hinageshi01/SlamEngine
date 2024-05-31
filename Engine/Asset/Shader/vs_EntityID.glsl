#version 460 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_modelViewProjection;

void main()
{
	gl_Position = u_modelViewProjection * vec4(a_position, 1.0);
}
