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
//out vec4 wNormal;
out vec4 World;
out vec4 Specular;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

uniform mat4 shadowMatrix;

void main()
{
	Diffuse = texture(texture_diffuse, vertex.TexCoord1);
	Normal = vec4((vertex.Normal.xyz + 1.0) * 0.5, 1.0);
	World = vertex.wPosition;
	Specular = texture(texture_specular, vertex.TexCoord1);
}