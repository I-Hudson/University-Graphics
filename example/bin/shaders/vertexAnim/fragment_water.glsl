#version 150

in Vertex
{
	vec4 Position;
	vec4 wPosition;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
	vec4 clipSpace;
	vec3 toCameraVector;
} vertex;


//Outputs
out vec4 Diffuse;
out vec4 Normal;
//out vec4 wNormal;
out vec4 World;
out vec4 Specular;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

uniform sampler2D texture_reflection;
uniform sampler2D texture_refraction;
uniform sampler2D texture_DUDVMap;
uniform sampler2D texture_waterDepth;

uniform float moveFactor;

const float distortionStrenght = 0.02;

void main()
{
	
	vec2 ndc = (vertex.clipSpace.xy / vertex.clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoord = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoord = vec2(ndc.x, ndc.y);

	float near = 0.1;
	float far = 1000.0;
	float depth = texture(texture_waterDepth, reflectTexCoord).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistnace = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	float waterDepth = floorDistance - waterDistnace;

	vec2 distortedTexCoords = texture(texture_DUDVMap, vec2(vertex.TexCoord1.x + moveFactor, vertex.TexCoord1.y)).rb * 0.1;
	distortedTexCoords = vertex.TexCoord1 + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 totalDistortion = (texture(texture_DUDVMap, distortedTexCoords).rb * 2.0 - 1.0) * distortionStrenght;

	refractTexCoord += totalDistortion;
	refractTexCoord = clamp(refractTexCoord, 0.01, 0.99);

	reflectTexCoord += totalDistortion;
	reflectTexCoord = clamp(reflectTexCoord, 0.01, 0.99);

	vec4 reflectColour = texture(texture_reflection, reflectTexCoord);
	vec4 refractColour = texture(texture_refraction, refractTexCoord);

	vec3 viewVector = normalize(vertex.toCameraVector);
	float refractiveFactor = dot(vertex.toCameraVector, vec3(0, 1, 0));

	vec4 normalMapColour = texture(texture_normal, totalDistortion);
	vec4 normal = vec4(normalMapColour.r * 2.0 - 1.0, normalMapColour.b, normalMapColour.g * 2.0 - 1.0, 0.0);
	normal = normalize(normal);

	Diffuse = mix(reflectColour, refractColour, 0.5);
	Diffuse = mix(Diffuse, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
	Diffuse.a = clamp(waterDepth / 0.5, 0, 1);

	Normal = vec4(normal.xyz, 1.0);
	World = vertex.wPosition;
	Specular = texture(texture_specular, vertex.TexCoord1);
}