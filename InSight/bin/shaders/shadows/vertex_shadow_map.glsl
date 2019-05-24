#version 400

in vec4 Position;
in vec4 Normal;
in vec2 TexCoord1;

out Vertex
{
	vec4 Pos;
	vec4 Normal;
	vec2 UV;
	vec4 ShadowCoord;
}vertex;

uniform mat4 model;
uniform mat4 ShadowMatrx;
uniform mat4 ProjectionView;

const mat4 SHADOW_BIAS_MATRIX = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

void main()
{
	vertex.Pos = model * Position;
	vertex.Normal = Normal;
	vertex.UV = TexCoord1;
	vertex.ShadowCoord = SHADOW_BIAS_MATRIX * ShadowMatrx * model * Position;

	gl_Position =  ProjectionView * model * Position;
}
