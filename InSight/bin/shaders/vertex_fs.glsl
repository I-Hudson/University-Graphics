#version 400

in vec4 Position;
in vec2 TexCoord1;

out Vertex
{
	vec2 TexCoord1;
} vertex;

void main()
{
	vertex.TexCoord1 = TexCoord1;
	gl_Position = Position;
}
