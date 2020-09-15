#version 450

#extension GL_NV_shadow_samplers_cube : enable

in vec3 texCoord;
uniform samplerCube PoolSkyCubeMap;
out vec4 fragColor;

void main()
{
	fragColor = textureCube(PoolSkyCubeMap, texCoord);
}
