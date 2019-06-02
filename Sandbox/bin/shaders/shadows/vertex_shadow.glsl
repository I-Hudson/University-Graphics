#version 400

in vec4 Position;
//in vec4 Color;
in vec4 Normal;
//in vec4 Tangent;
//in vec4 Bitangent;
in vec2 TexCoord1;

uniform mat4 model;
uniform mat4 ShadowProjectionView;

void main()
{
	gl_Position = ShadowProjectionView * model * Position;
}
