#version 450

in vec2 texCoord;
uniform sampler2D WaterHeightMap;
uniform float ODWNMR, TDWNMR;
out vec4 fragColor;

void main()
{
	float y[4];
	y[0] = texture2D(WaterHeightMap, texCoord.xy + vec2(ODWNMR, 0.0)).g;
	y[1] = texture2D(WaterHeightMap, texCoord.xy + vec2(0.0, ODWNMR)).g;
	y[2] = texture2D(WaterHeightMap, texCoord.xy - vec2(ODWNMR, 0.0)).g;
	y[3] = texture2D(WaterHeightMap, texCoord.xy - vec2(0.0, ODWNMR)).g;
	vec3 Normal = normalize(vec3(y[2] - y[0], TDWNMR, y[1] - y[3]));
	fragColor = vec4(Normal, 1.0);
}
