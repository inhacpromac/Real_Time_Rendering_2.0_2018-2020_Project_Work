#version 450 core

in vec4 vPosition;
vec4 vColor;
uniform float time;
uniform mat4 u_mvp_matrix;
out vec4 starColor;
float random_float();
void main(void)
{
    /*vColor.x = 0.8f + random_float() * 0.2f;
    vColor.y = 0.8f + random_float() * 0.2f;
    vColor.z = 0.8f + random_float() * 0.2f;
    vec4 newVertex = vPosition;
    newVertex.z += time;
    newVertex.z = fract(newVertex.z);
    float size = (20.0 * newVertex.z * newVertex.z);
    starColor = smoothstep(1.0, 7.0, size) * vColor;
    newVertex.z = (999.9 * newVertex.z) - 1000.0;*/
    gl_Position = u_mvp_matrix * vPosition;
    //gl_PointSize = 40.0;/*size;*/
}
float random_float()
{
    float res;
    unsigned int seed = 0x13371337;
    unsigned int tmp;
    seed *= 16807;
    tmp = seed ^ (seed >> 4) ^ (seed << 15);
    res = (tmp >> 9) | 0x3F800000;
    return (res - 1.0f);
}
