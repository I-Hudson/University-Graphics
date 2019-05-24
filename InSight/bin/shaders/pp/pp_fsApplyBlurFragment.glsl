#version 400

in Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

out vec4 FragColour;

//unifom
uniform sampler2D texture_diffuse;
uniform float exposure;
uniform float gamma;

void main()
{
	//uv
	vec2 uv = gl_FragCoord.xy / vec2(1920,1080);
	vec3 sceneColour = texture(texture_diffuse, uv).xyz;

	//map to ldr
	vec3 result = vec3(1.0) - exp(-sceneColour.xyz * exposure);
	result = pow(result, vec3(1.0 / gamma));
	FragColour = vec4(result, 1.0);
	//FragColour = vec4(sceneColour, 1.0);
}