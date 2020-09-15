#version 450

in vec4 vPosition;
out vec2 texCoord;

void main()
{
    texCoord.x = vPosition.x;
    texCoord.y = vPosition.y;
    gl_Position = vPosition * 2.0 - 1.0;
}
