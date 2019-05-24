#version 400

layout(triangles, equal_spacing, ccw) in;

in TessCont
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord1;
} tessCS[];

out TessEval
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord1;
} tessEval;

//uniforms
uniform sampler2D displaementTexture;
uniform float displacementScale;

void main()
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;
	vec3 p = gl_TessCoord.xyz;

	tessEval.Position = tessCS[0].Position * p.x + tessCS[1].Position * p.y + tessCS[2].Position * p.z;
	tessEval.Normal = normalize(tessCS[0].Normal * p.x + tessCS[1].Normal * p.y + tessCS[2].Normal * p.z);
	tessEval.TexCoord1 = tessCS[0].TexCoord1 * p.x + tessCS[1].TexCoord1 * p.y + tessCS[2].TexCoord1 * p.z;

	//apply displacement 
	float dist = texture(displaementTexture, tessEval.TexCoord1).r;

/*	tessEval.Position	= vec4(vec3(gl_TessCoord.x) * tessCS[0].Position.xyz + vec3(gl_TessCoord.y) * tessCS[1].Position.xyz + vec3(gl_TessCoord.z) * tessCS[2].Position.xyz, 1.0);
	tessEval.Normal		= vec4(vec3(gl_TessCoord.x) * tessCS[0].Normal.xyz + vec3(gl_TessCoord.y) * tessCS[1].Normal.xyz + vec3(gl_TessCoord.z) * tessCS[2].Normal.xyz, 0.0);
	tessEval.Normal		= normalize(tessEval.Normal);
	tessEval.TexCoord1	= vec2(gl_TessCoord.x) * tessCS[0].TexCoord1 + vec2(gl_TessCoord.y) * tessCS[1].TexCoord1 + vec2(gl_TessCoord.z) * tessCS[2].TexCoord1;
*/
	gl_Position = p0 * p.x + p1 * p.y + p2 * p.z;
	gl_Position.y += displacementScale * dist;
}