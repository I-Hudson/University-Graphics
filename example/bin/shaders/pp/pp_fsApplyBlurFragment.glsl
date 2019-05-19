#version 150

in Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

out vec4 FragColour;

uniform sampler2D texture_diffuse;

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(1920,1080);
	FragColour = vec4(texture(texture_diffuse, uv).xyz, 1.0);
}