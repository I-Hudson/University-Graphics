#version 400

in vec4 Position;
in vec2 TexCoord1;

out Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

//out vec2 blurTexCoords[11];
//
//uniform float HorSize;
//uniform float VerSize;
//uniform float BlurType;
//
//const int blurSize = 5;

void main()
{
	//if(BlurType == 0.0)
	//{
	//	vec2 centerTexCoord = Position.xy * 0.5 + 0.5;
	//	float pixelSize = 1.0 / HorSize;

	//	for(int i = -blurSize; i <= blurSize; i++)
	//	{
	//		blurTexCoords[i + blurSize] = centerTexCoord + vec2(pixelSize * i, 0.0);
	//	}
	//}
	//else
	//{
	//	vec2 centerTexCoord = Position.xy * 0.5 + 0.5;
	//	float pixelSize = 1.0 / VerSize;

	//	for(int i = -blurSize; i <= blurSize; i++)
	//	{
	//		blurTexCoords[i + blurSize] = centerTexCoord + vec2(0.0, pixelSize * i);
	//	}
	//}

	vertex.Position = Position;
	vertex.TexCoord1 = TexCoord1;
	gl_Position = Position;
}