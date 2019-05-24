#version 400

in TessEval
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord1;
} tessEval;

//Outputs
out vec4 Diffuse;
out vec4 Normal;
out vec4 World;

//uniforms
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

void main()
{
	Diffuse = texture(texture_diffuse, tessEval.TexCoord1);
	Normal = vec4((tessEval.Normal.xyz + 1.0) * 0.5, 1.0);
	World = tessEval.Position;
}