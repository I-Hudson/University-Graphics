#version 400

in Vertex
{
	vec4 Position;
	//vec4 Color;
	vec4 Normal;
	//vec4 Tangent;
	//vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

//uniforms
uniform vec4 cameraPos;

uniform vec4 lightDirection;
uniform vec4 lightColour;
uniform vec4 lightAmbient = vec4(0.4, 0.4, 0.4, 1.0);
uniform vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

uniform float specularPower = 1.0;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

void main()
{
	//phong
    vec4 ambient = lightAmbient;
	vec4 albedo = vertex.Normal;//texture(texture_diffuse, vertex.TexCoord1);
	//vec4 normalTex = texture(texture_normal, vertex.TexCoord1);// * 2.0 - 1.0;

	float Ndl = max(0.0, dot(vertex.Normal, -lightDirection));
	vec4 diffuse = lightColour * Ndl;
	
	vec4 R = reflect(lightDirection, vertex.Normal);
	vec4 E = normalize(cameraPos - vertex.Position);
	
	float specularTerm = pow(min(0.0, dot(R, E)), specularPower);
	specularTerm = min(0.0, specularTerm);
	vec4 specular = lightSpecular * specularTerm;
	
	FragColour = vec4(albedo.rgb, 1.0);//vec4(albedo.rgb * (ambient.rgb + diffuse.rgb + specular.rgb), 1.0);
}