#version 450 core

uniform  vec3 		ambientLight;
uniform  vec3 		diffuseLight;
uniform  vec3 		specularLight;

uniform  vec3 		ambientMaterial;
uniform  vec3 		diffuseMaterial;
uniform  vec3 		specularMaterial;
uniform  float 		shininess;
uniform  sampler2D 	textureSampler;

uniform float mixFactor;

uniform vec4 colorB;

in vec3 V;
in vec3 N;
in vec3 L;
in vec3 H;

in vec3 LightPosition;

in vec2 out_texcoord;

out vec4 FragColor;

void main(void)
{
	// compute cosine of the directions, using dot products
	// to see how much light would be reflected

	float diffuse  = max(0.0, dot(N, L));
	float specular = max(0.0, dot(N, H));

	// surfaces facing away from light (ngative dot product)
	// won't lit by directional light
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, shininess);
	
	vec3 scatteredLight = ambientLight  	 * ambientMaterial + (diffuse * diffuseLight) * diffuseMaterial;
	vec3 reflectedLight = (specular) * specularLight * specularMaterial;

	vec3 rgb   = min(scatteredLight + reflectedLight, vec3(1.0));
	
	vec4 colorA = vec4(rgb, 1.0) * texture(textureSampler, out_texcoord);

	FragColor = mix(colorA, colorB, mixFactor);

	//FragColor = vec4(1.0, 0.0, 0.0, alpha);
}