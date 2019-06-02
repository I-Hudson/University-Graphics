#version 400

in vec4 Position;

out Vertex
{
	vec4 Position;
} vertex;

uniform mat4 model;
uniform mat4 ProjectionView;

void main()
{
	vertex.Position = Position;
	gl_Position = ProjectionView * model * Position;
}
