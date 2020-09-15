#version 450 core

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 lightPosition;

out vec3 N;
out vec3 V;
out vec3 L;
out vec3 H;

out vec3 LightPosition;
out vec2 out_texcoord;

void main(void)
{
    // calculate view space position
    vec4 P        = viewMatrix * modelMatrix * vPosition;

    LightPosition = vec3(lightPosition);

    // calculate light vectors
    L   = vec3(lightPosition) - P.xyz;
    V   = -P.xyz;
    N   = mat3(modelMatrix * viewMatrix) * vNormal;
    
    // normalize
    L   = normalize(L);
    N   = normalize(N);
    V   = normalize(V);

    H   = L + V;

    gl_Position             = projectionMatrix * viewMatrix * modelMatrix * vPosition;

    out_texcoord            = vTexcoord;

    //gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
}
