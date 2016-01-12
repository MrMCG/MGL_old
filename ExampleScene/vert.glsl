#version 330 core

in vec3 position;
in vec2 texCoord;
in vec3 normals;
in vec4 colour;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out Vertex {
	vec2 texCoord;
	vec3 normals;
	vec4 colour;
} OUT;


void main(void)	{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	OUT.colour = colour;
	OUT.texCoord = texCoord;
	OUT.normals = normals;
}