#version 400

in vec4 Position;
in vec2 TexCoord1;

out Vertex
{
	vec4 Position;
	vec2 TexCoord1;
} vertex;

uniform mat4 model;

void main()
{
	vertex.Position = Position;
	vertex.TexCoord1 = (Position.xy + vec2(1.0)) * 0.5;

	gl_Position = Position;
}
