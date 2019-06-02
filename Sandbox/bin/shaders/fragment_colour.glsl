#version 150

in Vertex
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec4 Tangent;
	vec4 Bitangent;
	vec2 TexCoord1;
} vertex;

out vec4 FragColour;

uniform vec4 cameraPosition;
uniform vec4 lightDirection;

uniform vec4 lightColour = vec4(1.0, 0.85, 0.05, 1.0);;
uniform vec4 lightAmbient = vec4(0.4, 0.4, 0.4, 1.0);
uniform vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

uniform float specularPower = 32.0;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

void main()
{
	vec4 albedo = texture(texture_diffuse, vertex.TexCoord1);
	vec4 normal = texture(texture_normal, vertex.TexCoord1);// * 2.0 - 1.0;

	vec4 ambient = lightAmbient;

	float NdL = max(0.0, dot(normal, -lightDirection)); //get the lambertian term
	vec4 diffuse = lightColour * NdL; //Use the light colour as the diffuse colour of the light
	
	vec4 R = reflect(lightDirection, normal); //use the built in reflect function to reflect the light direction around the surface normal
	vec4 E = normalize(cameraPosition - vertex.Position); //get the directional vector from the vertex position (world space) to the camera position (world space)
	
	float specularTerm = pow(max(0.0, dot(R, E)), specularPower);
	vec4 specular = lightSpecular * specularTerm;
	
	FragColour = vec4(albedo.rgb * (ambient.rgb + diffuse.rgb + specular.rgb), 1.0);
}