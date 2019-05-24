#version 400

in vec4 Position;
//in vec4 Color;
in vec4 Normal;
//in vec4 Tangent;
//in vec4 Bitangent;
in vec2 TexCoord1;

out Vertex
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

//uniforms
uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 ProjectionView;
uniform vec4 cameraPos;

const vec4 plane = vec4(0, -1, 0, 15);

//uniform sampler2D waveTexture;
uniform vec4 waveData[3];
uniform float numOfPixels;

uniform float time;

const float  PI = 3.14159;

vec4 generateWave(vec4 aWave, vec4 aPosition, vec4 aTangent, vec4 aBinormal)
{
	//wave proporties
	float steepness = aWave.z;
	float wavelength = aWave.w;

	//positioin change
	vec4 p = aPosition;
	float k = 2 * PI / wavelength;
	float c = sqrt(9.8 / k);

	vec2 d = normalize(aWave.xy);
	float f = k * (dot(d, p.xz) - c * time);
	float a = steepness / k;

	//set tangent
	aTangent += vec4(
		-d.x * d.x * (steepness * sin(f)),
		d.x * (steepness * cos(f)),
		-d.x * d.y * (steepness * sin(f)),
		0.0);

	//set binormal
	aBinormal += vec4(
		-d.x * d.y * (steepness * sin(f)),
		d.y * (steepness * cos(f)),
		-d.y * d.y * (steepness * sin(f)),
		0.0
	);
	return vec4(
		d.x * (a *cos(f)),
		a * sin(f),
		d.y * (a * cos(f)),
		0.0
	);
}

void main()
{
	//tangent and binormal
	vec4 tangent = vec4(1, 0, 0, 0);
	vec4 binormal = vec4(0, 0, 1, 0);

	vec4 vertexPosition = Position;

	vec4 p = vertexPosition;
	for (int i = 0; i < numOfPixels; i++)
	{
		/*vec4 pixel = texture(waveTexture, vec2((1 / numOfPixels) * i, 0))*/;
		//p += generateWave(vec4(1, 0, 0.1, 20) , vertexPosition, tangent, binormal);
	}

	//apply wave distortion
	p += generateWave(vec4(1, 1, 0.05, 25), vertexPosition, tangent, binormal);
	vec4 normal = vec4(normalize(cross(binormal.xyz, tangent.xyz)), 1.0);

	p.w = 1.0;

	vertex.Position = p;
	vertex.wPosition = model * p;
	vertex.toCameraVector = cameraPos.xyz - vertex.wPosition.xyz;

	//vertex.Color = Color;
	vertex.Normal = normal;
	//vertex.Tangent = Tangent;
	//vertex.Bitangent = Bitangent;
	vertex.TexCoord1 = TexCoord1;
	vertex.clipSpace = ProjectionView * model * p;
	gl_Position = vertex.clipSpace;
}
