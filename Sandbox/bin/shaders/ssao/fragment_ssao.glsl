#version 150

in Vertex
{
	vec4 Position;
	vec2 TexCoord1;
} vertex;

out float FragColor;

//uniforms
uniform sampler2D ViewPosTexture;
uniform sampler2D NormalTexture;
uniform sampler2D randomMapTexture;

const vec2 crossKernal[4] = vec2[](vec2(1, 0), vec2(-1, 0), vec2(0, 1), vec2(0, -1));

struct AOData
{
	float radius;
	float bias;
	float intensity;
	float scale;
};
uniform AOData aoData;

float calculateOcclusion(in vec2 offset, in vec3 viewPosition, in vec3  viewNormal)
{
//get difference
	vec3 diff = texture(ViewPosTexture, vertex.TexCoord1 + offset).rgb - viewPosition;
	vec3 v = normalize(diff);

	float d = length(diff);

	//calc occlusion
	return max(0.0, dot(viewNormal, v) - aoData.bias) * (1.0 / (1.0 + d)) * aoData.intensity;
}

void main()
{
	//view position and view normal
	vec3 viewPosition = texture(ViewPosTexture, vertex.TexCoord1).rgb;
	vec3 viewNormal = texture(NormalTexture, vertex.TexCoord1).rgb * 2.0 - 1.0;

	//radius of sample
	float radius = aoData.radius / viewPosition.z;

	
	vec2 randomSize = textureSize(randomMapTexture, 0);
	vec2 rand = normalize(texture(randomMapTexture, gl_FragCoord.xy / randomSize).xy * 2.0 - 1.0);

	float ao = 0.0;
	int iterations = 4;

	//sample and calc occlusion
	for (int j = 0; j < iterations; ++j)
	{
		vec2 offset1 = crossKernal[j] * radius;

		vec2 offset2 = vec2(offset1.x * 0.707 - offset1.y * 0.707,
			offset1.x * 0.707 + offset1.y * 0.707);

		ao += calculateOcclusion(offset1 * 0.25, viewPosition, viewNormal);
		ao += calculateOcclusion(offset1 * 0.50, viewPosition, viewNormal);
		ao += calculateOcclusion(offset1 * 0.75, viewPosition, viewNormal);
		ao += calculateOcclusion(offset2, viewPosition, viewNormal);

	}

	ao /= iterations;

	FragColor = 1.0 - ao;

}
