#version 460 core

layout(location = 0) out int o_entityID;

uniform int u_entityID;

void main()
{
	o_entityID = u_entityID;
}
