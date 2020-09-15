#version 450

in vec2 texCoord;
uniform sampler2D WaterHeightMap;
uniform float DropRadius;
uniform vec2 Position;
out vec4 fragColor;
void main()
{
	vec2 vh = texture2D(WaterHeightMap, texCoord.xy).rg;
	float d = distance(texCoord.xy, Position);
	fragColor = vec4(vh.r, vh.g - 4.0f * max(DropRadius - d, 0.0), 0.0, 0.0);
}
