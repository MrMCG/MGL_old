#version 330 core

uniform sampler2D tex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
	vec3 normals;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	gl_FragColor = texture(tex, IN.texCoord) * IN.colour;
}