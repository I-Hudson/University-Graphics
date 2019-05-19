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

void main()
{
	Diffuse = vec4(0.4, 0.4, 1.0, 1.0);
	Normal = (vertex.Normal + 1.0) * 0.5;
	World = vertex.wPosition;
}