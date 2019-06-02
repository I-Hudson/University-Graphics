#version 400

in Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

out vec4 FragColour;

//uniforms
uniform sampler2D texture_Scene;
uniform sampler2D texture_bloom;
uniform float exposure;
uniform float gamma;

void main()
{
	//uv
	vec2 uv = gl_FragCoord.xy / vec2(1920, 1080);
	//scene colour (hdr), bloom colour (hdr blur)
	vec3 sceneColour = texture(texture_Scene, uv).xyz;
	vec3 bloomColour = texture(texture_bloom, uv).xyz;

	//additive blend scene and bloom
	sceneColour += bloomColour;

	//map to ldr
	vec3 result = vec3(1.0) - exp(-sceneColour.xyz * exposure);
	result = pow(result, vec3(1.0 / gamma));
	FragColour = vec4(result, 1.0);
}