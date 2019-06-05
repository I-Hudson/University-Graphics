#version 400

//Outputs
out vec4 FragColour;
out vec4 HDRColour;

//uniforms
uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;
uniform sampler2D WPosTex;
uniform sampler2D SpecTex;
uniform sampler2D ShadowTex;

uniform float IsDirLight;
uniform vec4 AmbientColour;
uniform vec4 CameraPos;

uniform float hdrOn;
uniform vec3 colour;
//directional light
struct DLight
{
	vec4 Position;
	vec4 LightDir;
	vec4 Diffuse;
	vec4 Specular;

	float Intensity;
};
uniform DLight dLight;

//point light
struct PLight
{
	vec4 Position;
	//float Constant;
	//float Linear;
	//float Quadratic;
	float Radius;

	//vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Intensity;
};
uniform PLight pLight;

//spot light 
struct SLight
{
	PLight baseLight;

	float Intensity;

	vec4 Direction;
	float InnerCutoff;
	float OuterCutoff;
};
uniform SLight sLight;

//calc the light colour value
//vec4 CalcLightInternal(PLight aLight, vec3 aLightDirection, vec3 aNormal, vec3 aWPos)
//{
//	//Ambient
//	vec4 ambientColour = vec4(AmbientColour.xyz, 1.0);
//
//	//Diffuse
//	float diffuseFactor = max(dot(aNormal, aLightDirection), 0.0);
//	vec4 diffuseColour = vec4(aLight.Diffuse.xyz * diffuseFactor, 1.0);
//
//	//Specular
//	vec3 viewDir = normalize(CameraPos.xyz - aWPos);
//	vec3 reflectDir = reflect(-aLightDirection, aNormal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
//	vec4 specular = vec4(1, 1, 1, 1) * spec;
//
//	//attenuation
//	float distance = length(aLight.Position.xyz - aWPos);
//	float attenuation = smoothstep(1.0, 0.0, distance / aLight.Radius);
//	//1.0 / (aLight.Constant + aLight.Linear * distance + aLight.Quadratic * (distance * distance));
//
//	ambientColour *= attenuation;
//	diffuseColour *= attenuation;
//	specular *= attenuation;
//
//	return vec4(diffuseColour.xyz, 1.0);
//}

//calc the directional light
vec4 CalcDirLight(DLight aLight, vec3 aWPos, vec3 aNormal)
{
	//Ambient
	vec3 ambient = AmbientColour.xyz;

	//Diffuse
	vec3 lightDir = normalize(aLight.Position.xyz - aWPos);
	vec3 normal = normalize(aNormal);

	//difference between lightDir and normal
	float diff = max(dot(lightDir, aNormal), 0.0);
	vec3 diffuse = diff * aLight.Diffuse.xyz;

	//Specular
	vec3 viewDir = normalize(CameraPos.xyz - aWPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), 128.0);

	vec3 specular = aLight.Specular.xyz * spec; // assuming bright white light color
	return vec4(ambient + diffuse + specular, 1.0) * aLight.Intensity;
}

//cal point light 
vec4 CalcPointLight(PLight aLight, vec3 aWPos, vec3 aNormal)
{
	//Ambient
	vec3 ambient = AmbientColour.xyz;

	//Diffuse
	vec3 lightDir = normalize(aLight.Position.xyz - aWPos);
	vec3 normal = normalize(aNormal);

	//difference between lightDir and normal
	float diff = max(dot(lightDir, aNormal), 0.0);
	vec3 diffuse = diff * aLight.Diffuse.xyz;

	//Specular
	vec3 viewDir = normalize(CameraPos.xyz - aWPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), 128.0);
	vec3 specular = aLight.Specular.xyz * spec; // assuming bright white light color

	//attenuation
	float distance = length(aLight.Position.xyz - aWPos);
	float radius = 1.0 - (distance / aLight.Radius);
	float attenuation = smoothstep(1.0, 0.0, 1.0 - radius);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4(ambient + diffuse + specular, 1.0) * aLight.Intensity;
}

//calc spot light 
vec4 CalcSpotLight(SLight aLight, vec3 aWPos, vec3 aNormal)
{
	//light direction
	//vec3 lightToPixel = normalize(aWPos - aLight.baseLight.Position.xyz);

	//Ambient
	vec3 ambient = AmbientColour.xyz;

	//Diffuse
	vec3 lightDir = normalize(aLight.baseLight.Position.xyz - aWPos);
	vec3 normal = normalize(aNormal);

	//difference between lightDir and normal
	float diff = max(dot(lightDir, aNormal), 0.0);
	vec3 diffuse = diff * aLight.baseLight.Diffuse.xyz;

	//Specular
	vec3 viewDir = normalize(CameraPos.xyz - aWPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), 128.0);

	vec3 specular = aLight.baseLight.Specular.xyz * spec; // assuming bright white light color
	vec4 colour = vec4(ambient + diffuse + specular, 1.0);

	float theta = dot(-lightDir, aLight.Direction.xyz);
	float epsilon = aLight.InnerCutoff - aLight.OuterCutoff;
	float intensity = clamp((theta - aLight.OuterCutoff) / epsilon, 0.0, 1.0);

	return (colour * intensity) * aLight.Intensity;

	//float spotFactor = dot(lightToPixel, aLight.Direction.xyz);
	//if (spotFactor > aLight.OuterCutoff)
	//{
	//	return colour;
	//}
	//return vec4(0, 0, 0, 0);
}

void main()
{
	//get space uv
	vec2 uv =  gl_FragCoord.xy / vec2(1920,1080);
	//get albedo colour
	vec3 albedo = texture(DiffuseTex, uv).xyz;
	//get the normal
	vec3 normal = normalize((texture(NormalTex, uv).xyz * 2.0) - 1.0).xyz;
	//get the world position
	vec3 wPos = texture(WPosTex, uv).xyz;
	//get the shadow tex value
	vec3 sTex = texture(ShadowTex, uv).xyz;

	vec3 spec = texture(SpecTex, uv).xyz;

	//overall light value
	vec4 lighting = vec4(0, 0, 0, 0);

	//light type 
	if(IsDirLight == 0.0)
	{
		lighting = CalcPointLight(pLight, wPos, normal);
	}
	else if(IsDirLight == 0.5)
	{
		lighting = CalcSpotLight(sLight, wPos, normal);
	}
	else
	{
		lighting = CalcDirLight(dLight, wPos, normal) * sTex.r;
	}

	//if hdrOn is on. map hdr value to ldr
	if(hdrOn == 1.0)
	{
		HDRColour = vec4(albedo * lighting.xyz, 1.0);
		vec3 colourMapped = HDRColour.xyz / (HDRColour.xyz + vec3(1.0));
		//vec3 colourMapped = vec3(1.0 - exp(-HDRColour * 25.0));
		colourMapped = pow(colourMapped, vec3(1.0 / 0.4));
		HDRColour = vec4(colourMapped, 1.0);
		//HDRColour = vec4(HDRColour.xyz / (HDRColour.xyz + vec3(1.0)), 1.0);
	}
	else
	{
		//hdr is unclamped
		HDRColour = vec4(albedo * lighting.xyz, 1.0);
	}
	//set FragColour
	FragColour = vec4(albedo * lighting.xyz, 1.0);
	FragColour = vec4(pow(FragColour.xyz, vec3(1.0 / 0.4)), 1.0);
}