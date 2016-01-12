#version 330 core

uniform sampler2D tex;

in Vertex {
	vec2 texCoord;
	vec3 normals;
	vec4 colour;
} IN;

out vec4 gl_Color;

void main(void)	{
	gl_Color = texture(tex, IN.texCoord) * IN.colour;
}