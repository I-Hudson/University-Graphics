#version 400

in vec4 Position;
in vec2 TexCoord1;

out Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

void main()
{
	vertex.Position = Position;
	vertex.TexCoord1 = TexCoord1;
	gl_Position = Position;
}