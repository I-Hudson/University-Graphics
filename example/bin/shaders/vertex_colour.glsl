#version 150

in vec4 Position;
in vec4 Color;
in vec4 Normal;
in vec4 Tangent;
in vec4 Bitangent;
in vec2 TexCoord1;

out Vertex
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec4 Tangent;
	vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

uniform mat4 model;
uniform mat4 ProjectionView;

void main()
{
	vertex.Position = model * Position;
	vertex.Color = Color;
	vertex.Normal = Normal;
	vertex.Tangent = Tangent;
	vertex.Bitangent = Bitangent;
	vertex.TexCoord1 = TexCoord1;

	gl_Position = ProjectionView * model * Position;
}
