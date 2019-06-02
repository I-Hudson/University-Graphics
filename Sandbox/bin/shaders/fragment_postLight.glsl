#version 400

in Vertex
{
	vec4 Position;
} vertex;

out vec4 FragColour;

uniform vec4 colour;

void main()
{
	FragColour = colour * 10;
}