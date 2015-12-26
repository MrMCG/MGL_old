#version 330 core

uniform sampler2D tex;
uniform sampler2D bump;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	gl_FragColor = mix(texture(tex, IN.texCoord), texture(bump, IN.texCoord), 0.5) * IN.colour;
}