#version 150

in vec4 Position;

uniform mat4 ProjectionView;

void main()
{
	gl_Position = ProjectionView * Position;
}
