// Targeting version 1.5 of GLSL. If the GLSL compiler does not support 
// 1.5 or newer it will cause an error.
#version 150

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings (but can cause an error on some machines)
precision mediump float;

struct Attenuation {

	float constant;
	float linear;
	float quadratic;
};


struct AmbientLight
{
    vec4 colorIntensity;
	
	bool enabled;
};

struct DirectionalLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 direction;

	bool enabled;
};


struct PositionalLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 position;

	Attenuation atten;

	bool enabled;
};

struct SpotLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 position;

	vec3 spotDirection;

	float spotCutoffCos;

	Attenuation atten;

	bool enabled;
};

uniform AmbientLight ambLight;
uniform DirectionalLight directLight;
uniform PositionalLight posLight;
uniform SpotLight spLight;

// Structure for material properties
struct Material
{
	vec4 ambientMat;

	vec4 diffuseMat;

	vec4 specularMat;

	float specularExp;

	vec4 emissiveMat;

	bool textureMapped;
};


uniform Material object;

uniform vec3 worldEyePosition;

// Vertex attributes passed in by the vertex shader
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;  

// Output color for the fragment (pixel)
out vec4 FragColor;

vec4 calcAmbientLight( AmbientLight ambLight, Material object )
{
	vec4 totalAmbientLight = vec4(0.0f,0.0f,0.0f,0.0f);
	
	if(ambLight.enabled == true) {
 
		totalAmbientLight = object.ambientMat * ambLight.colorIntensity; 
	}

	return totalAmbientLight;
}

vec4 calcDirectionalLight( DirectionalLight directLight, Material object )
{
	vec4 totalDirectionalLight = vec4(0.0f,0.0f,0.0f,0.0f);

	 
	 if(directLight.enabled == true) {
 
		// Diffuse
		float DiffuseFactor = max(dot(Normal0, directLight.direction),0.0f);
 
		vec4 diffuseTerm = DiffuseFactor * object.diffuseMat * directLight.colorIntensity;

		 // Specular
		vec3 VertexToEye = normalize(worldEyePosition - WorldPos0);
 
		vec3 LightReflect = normalize(reflect(-directLight.direction, Normal0));
 
		float SpecularFactor = max(dot(VertexToEye, LightReflect), 0.0f);
 
		SpecularFactor = pow( SpecularFactor, object.specularExp);
 
		vec4 specularTerm = SpecularFactor * object.specularMat * directLight.specularIntensity;
 
		totalDirectionalLight = diffuseTerm + specularTerm;

	 }


	return totalDirectionalLight;

}

vec4 calcPositionalLight( PositionalLight posLight, Material object )
{
	vec4 totalPositionalLight = vec4(0.0f,0.0f,0.0f,0.0f);

	 // Point Light
	if(posLight.enabled == true) {
 
	// Find a unit vector that points at the light source
	vec3 directionToPosLight = normalize( posLight.position - WorldPos0 );
 
	// Diffuse
	float DiffuseFactor = max(dot(Normal0, directionToPosLight), 0.0f);
	vec4 diffuseTerm = DiffuseFactor * object.diffuseMat * posLight.colorIntensity;
 
	// Specular
	vec3 VertexToEye = normalize( worldEyePosition - WorldPos0);
	vec3 LightReflect = normalize( reflect(-directionToPosLight, Normal0) );
 
	float SpecularFactor = max( dot( VertexToEye, LightReflect ), 0.0f);
 
	SpecularFactor = pow(SpecularFactor, object.specularExp);
 
	vec4 specularTerm = SpecularFactor * object.specularMat * posLight.specularIntensity;
 
	totalPositionalLight = diffuseTerm + specularTerm; 
 
 }


	return totalPositionalLight;
}

vec4 calcSpotLight( SpotLight spLight, Material object)
{
	vec4 totalSpotLight = vec4(0.0f,0.0f,0.0f,0.0f);
 
	 // SpotLight
	 if(spLight.enabled == true) {
 
		vec3 LightToPixel = normalize(WorldPos0 - spLight.position); 
 
		float spotCosFactor = dot(LightToPixel, spLight.spotDirection); 
 
		if( spotCosFactor > spLight.spotCutoffCos ) {
 
			vec3 directionToSpotLight = - LightToPixel;
 
			 // Diffuse
			 float DiffuseFactor = max(dot(Normal0, directionToSpotLight), 0.0f); 
			 vec4 diffuseTerm = DiffuseFactor * object.diffuseMat * spLight.colorIntensity;
 
			 // Specular
			 vec3 VertexToEye = normalize(worldEyePosition - WorldPos0);
			 vec3 LightReflect = normalize(reflect(-directionToSpotLight, Normal0));
			 float SpecularFactor = max(dot(VertexToEye, LightReflect), 0.0f);
 
			 SpecularFactor = pow(SpecularFactor, object.specularExp);
 
			 vec4 specularTerm = SpecularFactor *  object.specularMat * spLight.specularIntensity;

			 totalSpotLight = diffuseTerm + specularTerm;
 
			 // Traditional method for attenuating illumination
			 // near the edges of the cone
			 float falloff = pow( spotCosFactor, 100.0f );
 
			 // Alternative method for attenuating illumination
			 // near the edges of the cone
			 //float falloff = 1.0f - (1.0f - spotCosFactor) / (1.0f - spLight.spotCutoffCos);
 
			 totalSpotLight = falloff * totalSpotLight;
		}
	 }

	return totalSpotLight;
}

uniform sampler2D gSampler;

void main()
{
	FragColor = object.emissiveMat;

	Material texturedMaterial = object;

	FragColor = calcAmbientLight( ambLight, texturedMaterial ) + 
	            calcDirectionalLight( directLight, texturedMaterial ) + 
				calcPositionalLight( posLight, texturedMaterial ) + 
				calcSpotLight( spLight, texturedMaterial ) + 
				object.emissiveMat;

	if (object.textureMapped) {
		if (object.textureMapped) {
			texturedMaterial.ambientMat = texture2D(gSampler, TexCoord0.st);
			texturedMaterial.diffuseMat = texture2D(gSampler, TexCoord0.st);
		}

		FragColor = calcAmbientLight( ambLight, texturedMaterial ) + 
	            calcDirectionalLight( directLight, texturedMaterial ) + 
				calcPositionalLight( posLight, texturedMaterial ) + 
				calcSpotLight( spLight, texturedMaterial ) + 
				object.emissiveMat;
	}
	else
	{
		FragColor = calcAmbientLight( ambLight, object ) + 
	            calcDirectionalLight( directLight, object ) + 
				calcPositionalLight( posLight, object ) + 
				calcSpotLight( spLight, object ) + 
				object.emissiveMat;
	}

}