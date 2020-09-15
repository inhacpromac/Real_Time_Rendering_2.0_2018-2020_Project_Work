#version 450

in vec4 vPosition;
uniform mat4 u_mvp_matrix;
out vec3 texCoord;

void main()
{
    texCoord.xyz = vec3(vPosition.x, -vPosition.yz);
    gl_Position = u_mvp_matrix * vPosition;
}
