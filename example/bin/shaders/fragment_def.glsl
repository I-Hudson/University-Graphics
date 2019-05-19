#version 150

in Vertex
{
	vec4 Position;
	vec4 wPosition;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

//Outputs
out vec4 Diffuse;
out vec4 Normal;
out vec4 World;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

void main()
{
	Diffuse = texture(texture_diffuse, vertex.TexCoord1);
	Normal = vec4((vertex.Normal.xyz + 1.0) * 0.5, 1.0);
	World = vertex.wPosition;
}