#version 150

in Vertex
{
	vec4 Pos;
	vec4 Normal;
	vec2 UV;
	vec4 ShadowCoord;
}vertex;

uniform vec4 lightDir;

out vec4 FragColour;

uniform sampler2D texture_worldPos;
uniform sampler2D texture_shadow;

uniform float LightType;

const float SHADOW_BIAS = 0.002f;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);

void main()
{
	float mapSize = 1024;
	float texelSize = 1.0 / mapSize;

	float shadowFactor = 1;
	float bias = SHADOW_BIAS * tan(acos(dot(vertex.Normal.xyz, lightDir.xyz)));
	bias = clamp(bias, 0, 0.01);

	vec3 worldPosition = texture(texture_worldPos, gl_FragCoord.xy / vec2(1920, 1080)).xyz;

	if(LightType == 0.0)
	{
		for (int i = 0; i < 4; i++)
		{        
			//  - A random sample, based on the pixel's screen location.
			//    No banding, but the shadow moves with the camera, which looks weird.
			int index = int(16.0 * randomNumber(floor(worldPosition * 1000.0), i)) % 16;
			if (texture(texture_shadow, vertex.ShadowCoord.xy + poissonDisk[index] / 700.0).z < vertex.ShadowCoord.z - bias)
			{
				shadowFactor -= 0.2;
			}
		}
	}
	else if(LightType == 1.0)
	{	
		if(vertex.ShadowCoord.x / vertex.ShadowCoord.w >= 0 && vertex.ShadowCoord.x / vertex.ShadowCoord.w <= 1 && 
			vertex.ShadowCoord.y / vertex.ShadowCoord.w >= 0 && vertex.ShadowCoord.y / vertex.ShadowCoord.w <= 1)
		{
			if(texture(texture_shadow, (vertex.ShadowCoord.xy / vertex.ShadowCoord.w)).z < (vertex.ShadowCoord.z - SHADOW_BIAS) / vertex.ShadowCoord.w)
			{
				shadowFactor = 0;
			}
		}
	}

	FragColour = vec4(1, 1, 1, 1) * shadowFactor;
}