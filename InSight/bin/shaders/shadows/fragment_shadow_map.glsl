#version 400

in Vertex
{
	vec4 Pos;
	vec4 Normal;
	vec2 UV;
	vec4 ShadowCoord;
}vertex;

uniform vec4 lightDir;

out vec4 FragColour;

//uniforms
uniform sampler2D texture_worldPos;
uniform sampler2D texture_shadow;
uniform mat4 ShadowMatrx;
uniform float LightType;

const float SHADOW_BIAS = 0.005f;
const mat4 SHADOW_BIAS_MATRIX = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

const vec2 poissonDisk[16] = vec2[](
	vec2(-0.94201624, -0.39906216),
	vec2(0.94558609, -0.76890725),
	vec2(-0.094184101, -0.92938870),
	vec2(0.34495938, 0.29387760),
	vec2(-0.91588581, 0.45771432),
	vec2(-0.81544232, -0.87912464),
	vec2(-0.38277543, 0.27676845),
	vec2(0.97484398, 0.75648379),
	vec2(0.44323325, -0.97511554),
	vec2(0.53742981, -0.47373420),
	vec2(-0.26496911, -0.41893023),
	vec2(0.79197514, 0.19090188),
	vec2(-0.24188840, 0.99706507),
	vec2(-0.81409955, 0.91437590),
	vec2(0.19984126, 0.78641367),
	vec2(0.14383161, -0.14100790)
	);

float randomNumber(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{
	float shadowFactor = 1;
	//set bias depending on vertex normal and light dir
	float bias = SHADOW_BIAS * tan(acos(dot(vertex.Normal.xyz, -lightDir.xyz)));
	bias = clamp(bias, 0.0, 0.005);

	//get world position
	vec3 worldPosition = texture(texture_worldPos, gl_FragCoord.xy / vec2(1920, 1080)).xyz;

	if(LightType == 0.0)
	{
		for (int i = 0; i < 4; i++)
		{        
			//generate a random number
			int index = int(16.0 * randomNumber(floor(worldPosition * 1000.0), i)) % 16;
			//shadowFactor -= 0.35 * (1.0 - texture(texture_shadow, vertex.ShadowCoord.xy + poissonDisk[index] / 700.0).r);

			if (texture(texture_shadow, vertex.ShadowCoord.xy + poissonDisk[index] / 700.0).z < vertex.ShadowCoord.z - bias)
			{
				shadowFactor -= 1.0 * 0.15;
			}
		}
	}
	else if(LightType == 1.0)
	{	
		//perspective shadows(point & spot lights)
		//if(vertex.ShadowCoord.x / vertex.ShadowCoord.w >= 0 && vertex.ShadowCoord.x / vertex.ShadowCoord.w <= 1 && 
		//	vertex.ShadowCoord.y / vertex.ShadowCoord.w >= 0 && vertex.ShadowCoord.y / vertex.ShadowCoord.w <= 1)
		//{
		//	if(texture(texture_shadow, (vertex.ShadowCoord.xy / vertex.ShadowCoord.w)).z < (vertex.ShadowCoord.z - SHADOW_BIAS) / vertex.ShadowCoord.w)
		//	{
		//		shadowFactor = 0;
		//	}
		//}
	}

	FragColour = vec4(1, 1, 1, 1) * shadowFactor;
}