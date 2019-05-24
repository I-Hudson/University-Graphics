#version 400

layout(vertices = 3) out;

in Vertex
{
	vec4 Position;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex[];

out TessCont
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord1;
} tessCS[];

//uniforms
uniform vec4 CameraPos;
uniform float TessHighDetailDist;
uniform float TessHighDetailValue;
uniform float TessMediumDetailDist;
uniform float TessMediumDetailValue;
uniform float TesslowDetailValue;


//Get the tessellation value based on the distance to the 
//camera
float GetTessDistance(float aDistance0, float aDistance1)
{
	//Get the mid distance from aDistance0 and aDistance1
	float midDistance = (aDistance0 + aDistance1) * 0.5;
	//If we are within TessHighDetailDist
	//return TessHighDetailValue as we want high detail 
	if (midDistance < TessHighDetailDist)
	{
		return TessHighDetailValue;
	}
	//if we are between 3TessHighDetailDist0 and 10TessMediumDetailDist0
	//then scale the detail on the distance
	else if (midDistance < TessMediumDetailDist)
	{
		return TessMediumDetailValue;
		//tess is the max distance - the current distance
		//float tess = 10 - (TessMediumDetailDist - TessHighDetailDist);//TessMediumDetailValue - midDistance;
		//return clamp(tess, 1.0, TessMediumDetailValue);
				//clamp(tess, 1.0, 10.0);
	}
	//If we are further than 10.0 units the n return 1.0
	return TesslowDetailValue;
}

void main()
{
	//Set the gl_out to the gl_in
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	//Set all the tessCS to the corresponding vertex
	tessCS[gl_InvocationID].Position	= vertex[gl_InvocationID].Position;
	tessCS[gl_InvocationID].Normal		= vertex[gl_InvocationID].Normal;
	tessCS[gl_InvocationID].TexCoord1	= vertex[gl_InvocationID].TexCoord1;

	//Calculate the distance form the camera's position to the controls points
	float vertexDistance0 = distance(CameraPos.xyz, tessCS[0].Position.xyz);
	float vertexDistance1 = distance(CameraPos.xyz, tessCS[1].Position.xyz);
	float vertexDistance2 = distance(CameraPos.xyz, tessCS[2].Position.xyz);

	//Set the outer level values
	gl_TessLevelOuter[0] = GetTessDistance(vertexDistance1, vertexDistance2);
	gl_TessLevelOuter[1] = GetTessDistance(vertexDistance2, vertexDistance0);
	gl_TessLevelOuter[2] = GetTessDistance(vertexDistance0, vertexDistance1);
						   
	//Set the inner level values
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}