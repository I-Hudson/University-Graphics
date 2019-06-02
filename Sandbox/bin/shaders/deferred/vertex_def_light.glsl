#version 150

in vec4 Position;

uniform float IsDirLight;
uniform mat4 ProjectionView;


void main()
{
	if(IsDirLight < 1.0)
	{
		gl_Position = ProjectionView * Position;
	}
	else
	{
		gl_Position = Position;
	}
}
