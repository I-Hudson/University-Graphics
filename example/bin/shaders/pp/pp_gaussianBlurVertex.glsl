#version 150

in vec4 Position;
in vec2 TexCoord1;

out vec2 blurTexCoords[11];

uniform float HorSize;
uniform float VerSize;
uniform float BlurType;

void main()
{
	if(BlurType == 0.0)
	{
		vec2 centerTexCoord = Position.xy * 0.5 + 0.5;
		float pixelSize = 1.0 / HorSize;

		for(int i = -5; i <= 5; i++)
		{
			blurTexCoords[i + 5] = centerTexCoord + vec2(pixelSize * i, 0.0);
		}
	}
	else
	{
		vec2 centerTexCoord = Position.xy * 0.5 + 0.5;
		float pixelSize = 1.0 / VerSize;

		for(int i = -5; i <= 5; i++)
		{
			blurTexCoords[i + 5] = centerTexCoord + vec2(0.0, pixelSize * i);
		}
	}
	gl_Position = Position;
}