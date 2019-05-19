#version 150

in vec2 blurTexCoords[11];

out vec4 FragColour;

uniform sampler2D texture_blur;

void main()
{
	FragColour = vec4(0.0);
	FragColour += texture(texture_blur, blurTexCoords[0]) * 0.0093;
	FragColour += texture(texture_blur, blurTexCoords[1]) * 0.028002;
	FragColour += texture(texture_blur, blurTexCoords[2]) * 0.065984;
	FragColour += texture(texture_blur, blurTexCoords[3]) * 0.121703;
	FragColour += texture(texture_blur, blurTexCoords[4]) * 0.175713;
	FragColour += texture(texture_blur, blurTexCoords[5]) * 0.198596;
	FragColour += texture(texture_blur, blurTexCoords[6]) * 0.175713;
	FragColour += texture(texture_blur, blurTexCoords[7]) * 0.121703;
	FragColour += texture(texture_blur, blurTexCoords[8]) * 0.065984;
	FragColour += texture(texture_blur, blurTexCoords[9]) * 0.028002;
	FragColour += texture(texture_blur, blurTexCoords[10]) * 0.0093;
	FragColour.w = 1.0;
}