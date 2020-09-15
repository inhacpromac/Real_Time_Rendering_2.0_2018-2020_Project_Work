#version 450

in vec2 texCoord;
uniform sampler2D WaterHeightMap;
uniform float ODWHMR;
out vec4 fragColor;

void main()
{
	vec2 vh = texture2D(WaterHeightMap, texCoord.xy).rg;
	float force = 0.0;
	/*Considering Mass as 0.707107*/
	force += 0.707107 * (texture2D(WaterHeightMap, texCoord.xy - vec2(ODWHMR, ODWHMR)).g - vh.g);
	force += texture2D(WaterHeightMap, texCoord.xy - vec2(0.0, ODWHMR)).g - vh.g;
	force += 0.707107 * (texture2D(WaterHeightMap, texCoord.xy + vec2(ODWHMR, -ODWHMR)).g - vh.g);
	force += texture2D(WaterHeightMap, texCoord.xy - vec2(ODWHMR, 0.0)).g - vh.g;
	force += texture2D(WaterHeightMap, texCoord.xy + vec2(ODWHMR, 0.0)).g - vh.g;
	force += 0.707107 * (texture2D(WaterHeightMap, texCoord.xy + vec2(-ODWHMR, ODWHMR)).g - vh.g);
	force += texture2D(WaterHeightMap, texCoord.xy + vec2(0.0, ODWHMR)).g - vh.g;
	force += 0.707107 * (texture2D(WaterHeightMap, texCoord.xy + vec2(ODWHMR, ODWHMR)).g - vh.g);
	force *= 0.125;
	vh.r += force;
	vh.g += vh.r;
	vh.g *= 0.99;
	fragColor = vec4(vh, 0.0, 0.0);
}
