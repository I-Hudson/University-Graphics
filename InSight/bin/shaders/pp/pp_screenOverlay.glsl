#version 400

in Vertex
{
	vec4 Position;
	//vec4 Color;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

void main()
{
	FragColour = vec4(1, 0, 0, 1);
}