#version 400
out float FragDepth;

void main()
{
	FragDepth = gl_FragCoord.z;
}