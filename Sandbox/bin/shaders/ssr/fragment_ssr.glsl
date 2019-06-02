#version 400

in Vertex
{
	vec4 Position;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

//uniforms
uniform vec4 CameraPos;
uniform sampler2D texture_albedo;
uniform sampler2D texture_wPosition;
uniform sampler2D texture_normal;
uniform sampler2D texture_depth;
uniform mat4 invView;
uniform mat4 Projection;
uniform mat4 invProjection;
uniform mat4 view;
uniform mat4 normalMatrix;
uniform mat4 invModel;

//consts
const float step = 0.1;
const float minRayStep = 0.1;
const int maxSteps = 30;
const float searchDist = 5;
const int numBinarySearchSteps = 5;
const float relfectionFalloff = 3.0;

const vec3 Scale = vec3(0.8);
const float K = 19.19;

//functions
vec4 RayCast(in vec3 aDir, inout vec3 aHitCoord, out float aDepth, in vec3 aViewPos);
vec3 BinarySearch(inout vec3 aDir, inout vec3 aHitCoord, inout float aDepth, in vec3 aViewPos);
vec3 fresnelSchlick(in float aCosTheta, in vec3 F0);
vec3 has(vec3 aHash);


//implemtation based on (http://imanolfotia.com/blog/update/2017/03/11/ScreenSpaceReflections.html)
void main()
{
	//uv
	vec2 uv = gl_FragCoord.xy / vec2(1920, 1080);
	//world position
	vec3 wPos = texture(texture_wPosition, uv).xyz;
	//world
	vec3 normal = (texture(texture_normal, uv).xyz * 2.0) - 1.0;	//Might need to be in view space 
	//loacl
	//normal = normalize(vec3(invModel * vec4(normal, 1.0)).xyz);
	//view
	vec4 viewNormal = normalMatrix * vec4(normal, 1.0);
	//colour image
	vec3 albedo = texture(texture_albedo, uv).xyz;
	//position in view space
	vec3 viewPos = wPos.xyz;

	//get the reflection direction
	vec3 relfectDir = normalize(reflect(normalize(wPos), normalize(normal)));
	vec3 hitPos = viewPos;
	float dDepth;

	//reflected tex coords 
	vec4 coords = RayCast(relfectDir * max(minRayStep, -viewPos.z), hitPos, dDepth, viewPos);

	//reflected coords
	vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5) - coords.xy));

	//smooth edges at the edges of the screen
	float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

	//screen space reflection. Sample colour texture
	vec3 SSR = texture(texture_albedo, coords.xy).rgb;

	FragColour = vec4(SSR, 1);
}

vec4 RayCast(in vec3 aDir, inout vec3 aHitCoord, out float aDepth, in vec3 aViewPos)
{
	//advance along ray
	aDir *= step;

	//depth
	float depth = 0.0;
	//curr step
	int steps = 0;
	//coords
	vec4 projectCoords = vec4(0.0);

	for (int i = 0; i < maxSteps; i++)
	{
		//curr pos += reflect direciton
		aHitCoord += aDir;
		projectCoords = Projection * vec4(aHitCoord, 1.0);
		//perspective divide
		projectCoords.xy /= projectCoords.w;
		projectCoords.xy = projectCoords.xy * 0.5 + 0.5;

		//depth
		depth = aViewPos.z;

		//if depth is to far continue
		if (depth > 1000.0)
		{
			continue;
		}

		//diff depth
		aDepth = aHitCoord.z - depth;

		//depth check
		if ((aDir.z - aDepth) < 1.2)
		{
			if (aDepth <= 0.0)
			{
				vec4 result;
				result = vec4(BinarySearch(aDir, aHitCoord, aDepth, aViewPos), 1.0);
				return result;
			}
		}
		steps++;
	}

	return vec4(projectCoords.xy, depth, 1.0);
}

//search
vec3 BinarySearch(inout vec3 aDir, inout vec3 aHitCoord, inout float aDepth, in vec3 aViewPos)
{
	float depth;
	vec4 projectCoord;
	for (int i = 0; i < numBinarySearchSteps; i++)
	{
		//coords in projection view
		projectCoord = Projection * vec4(aHitCoord, 1.0);
		projectCoord.xy /= projectCoord.w;
		projectCoord.xy = projectCoord.xy * 0.5 + 0.5;

		//depth
		depth = aViewPos.z;

		//diff in depth
		aDepth = aHitCoord.z - depth;
		//half direction
		aDir *= 0.5;

		//check if depth is closer or futher away
		if (aDepth > 0.0)
		{
			aHitCoord += aDir;
		}
		else
		{
			aHitCoord -= aDir;
		}
	}

	projectCoord = Projection * vec4(aHitCoord, 1.0);
	projectCoord.xy /= projectCoord.w;
	projectCoord.xy = projectCoord.xy * 0.5 + 0.5;


	return vec3(projectCoord.xy, depth);
}