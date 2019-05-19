#version 150

in Vertex
{
	vec4 Position;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

void main()
{
	FragColour = texture(texture_diffuse, vertex.TexCoord1);
}