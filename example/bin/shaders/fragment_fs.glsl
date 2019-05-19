#version 150

in Vertex
{
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D WPosTex;
uniform sampler2D FinalTex;
uniform sampler2D ShadowTex;

const vec2 ScreenSize = vec2(1920 / 1080);

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(1920,1080);
	vec3 albedo = texture(DiffuseTex, uv).xyz;
	vec3 normal = normalize((texture(NormalTex, uv).xyz * 2.0) - 1.0);
	vec3 wPos = texture(WPosTex, uv).xyz;
	vec3 shadow = texture(ShadowTex, uv).xyz;

	vec3 finalTex = texture(FinalTex, uv).xyz;

	FragColour = vec4(finalTex, 1.0);
}
