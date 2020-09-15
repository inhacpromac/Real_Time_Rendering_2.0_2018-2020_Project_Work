#version 450

uniform mat4 u_mvp_matrix;
uniform sampler2D WaterHeightMap;
in vec3 vPosition;
out vec2 texCoord;
out vec3 Position;

void main()
{
    texCoord.xy = vec2(vPosition.x * 0.5 + 0.5, 0.5 - vPosition.z * 0.5);
    Position = vPosition.xyz;
    Position.y += texture2D(WaterHeightMap, texCoord.xy).g;
    gl_Position = u_mvp_matrix * vec4(Position, 1.0);
}
