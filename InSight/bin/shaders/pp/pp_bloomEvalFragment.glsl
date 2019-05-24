#version 400

in Vertex
{
	vec4 Position;
	vec2 TexCoord1;
}vertex;

out vec4 FragColour;

//uniform
uniform sampler2D texture_Input;

void main()
{
	//uv
	vec2 uv = gl_FragCoord.xy / vec2(1920, 1080);
	//fragment colour
	vec4 FragmentColour = texture(texture_Input, uv);
	//get brightness value
	float brightness = dot(FragmentColour.xyz, vec3(0.2126, 0.7152, 0.0722));
		//(FragmentColour.r * 0.2126) + (FragmentColour.b * 0.7152) + (FragmentColour.b * 0.0722);
	//if brightness is above 1.0
	//set fragColour to FragmentColour.rbg
	if (brightness > 2.0)
	{
		FragColour = vec4(FragmentColour.rgb, 1.0);
	}
	else
	{
		FragmentColour = vec4(0.0, 0.0, 0.0, 0.0);
	}
}