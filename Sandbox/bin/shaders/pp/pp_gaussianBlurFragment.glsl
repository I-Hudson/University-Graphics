#version 400

//in vec2 blurTexCoords[11];
in Vertex
{
	vec4 Position;
vec2 TexCoord1;
}vertex;

out vec4 FragColour;

//uniforms
uniform sampler2D texture_blur;
//uniform float HorSize;
//uniform float VerSize;
uniform float BlurType;

//blur weights
const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	FragColour = vec4(0.0);

	//vec2 uv = gl_FragCoord.xy / vec2(1920, 1080);
	//texture offset, get size of a single fragment
	vec2 tex_offset = 1.0 / textureSize(texture_blur, 0);
	// current fragment's contribution
	vec3 result = texture(texture_blur, vertex.TexCoord1).rgb * weight[0]; 

	if(BlurType == 0.0)
	{
		for(int i = 1; i < 5; ++i)
       {
		   //blur result
           result += texture(texture_blur, vertex.TexCoord1 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
           result += texture(texture_blur, vertex.TexCoord1 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
       }
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
		   //blur result
			result += texture(texture_blur, vertex.TexCoord1 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(texture_blur, vertex.TexCoord1 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColour = vec4(result, 1.0);

	//blur FragColour
	//FragColour += texture(texture_blur, blurTexCoords[0]) * 0.0093;
	//FragColour += texture(texture_blur, blurTexCoords[1]) * 0.028002;
	//FragColour += texture(texture_blur, blurTexCoords[2]) * 0.065984;
	//FragColour += texture(texture_blur, blurTexCoords[3]) * 0.121703;
	//FragColour += texture(texture_blur, blurTexCoords[4]) * 0.175713;
	//FragColour += texture(texture_blur, blurTexCoords[5]) * 0.198596;
	//FragColour += texture(texture_blur, blurTexCoords[6]) * 0.175713;
	//FragColour += texture(texture_blur, blurTexCoords[7]) * 0.121703;
	//FragColour += texture(texture_blur, blurTexCoords[8]) * 0.065984;
	//FragColour += texture(texture_blur, blurTexCoords[9]) * 0.028002;
	//FragColour += texture(texture_blur, blurTexCoords[10]) * 0.0093;
	//FragColour.w = 1.0;
}