/*#version 450

#extension GL_NV_shadow_samplers_cube : enable

in vec2 texCoord;
in vec3 Position;
uniform sampler2D WaterNormalMap;
uniform samplerCube PoolSkyCubeMap;
uniform vec3 LightPosition, CubeMapNormals[6], CameraPosition;
vec3 fragColorCalc;
out vec4 fragColor;

vec3 IntersectCubeMap(vec3 Position, vec3 Direction)
{
	vec3 Point;
	for (int i = 0; i < 6; i++)
	{
		float NdotR = -dot(CubeMapNormals[i], Direction);
		if (NdotR > 0.0)
		{
			float Distance = (dot(CubeMapNormals[i], Position) + 1.0) / NdotR;
			if (Distance > -0.03)
			{
				Point = Direction * Distance + Position;
				if (Point.x > -1.001 && Point.x < 1.001 && Point.y > -1.001 && Point.y < 1.001 && Point.z > -1.001 && Point.z < 1.001)
				{
					break;
				}
			}
		}
	}
	return vec3(Point.x, -Point.yz);
}

void main()
{
	vec3 Normal = normalize(texture2D(WaterNormalMap, texCoord.xy).rgb);
	vec3 Direction = normalize(Position - CameraPosition);
	vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
	vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, refract(Direction, Normal, 0.750395))).rgb;
	vec3 LightDirectionReflected = reflect(normalize(Position - LightPosition), Normal);
	float Specular = pow(max(-dot(Direction, LightDirectionReflected), 0.0), 128);
	fragColorCalc = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction)) + Specular;
	fragColor = vec4(fragColorCalc, 1.0);
}*/


#version 450

#extension GL_NV_shadow_samplers_cube : enable

in vec2 texCoord;
in vec3 Position;
uniform sampler2D WaterNormalMap;
uniform samplerCube PoolSkyCubeMap;
uniform vec3 LightPosition, CubeMapNormals[6], CameraPosition;
vec3 fragColorCalc;
out vec4 fragColor;
vec3 IntersectCubeMap(vec3 Position, vec3 Direction)
{
	vec3 Point;
	for (int i = 0; i < 6; i++)
	{
		float NdotR = -dot(CubeMapNormals[i], Direction);
		if (NdotR > 0.0)
		{
			float Distance = (dot(CubeMapNormals[i], Position) + 1.0) / NdotR;
			if (Distance > -0.03)
			{
				Point = Direction * Distance + Position;
				if (Point.x > -1.001 && Point.x < 1.001 && Point.y > -1.001 && Point.y < 1.001 && Point.z > -1.001 && Point.z < 1.001)
				{
					break;
				}
			}
		}
	}
	return vec3(Point.x, -Point.yz);
}

void main()
{
	vec3 Normal = normalize(texture2D(WaterNormalMap, texCoord.xy).rgb);
	vec3 Direction = normalize(Position - CameraPosition);
	if (CameraPosition.y > 0)
	{
		vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
		vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, refract(Direction, Normal, 0.750395))).rgb;
		vec3 LightDirectionReflected = reflect(normalize(Position - LightPosition), Normal);
		float Specular = pow(max(-dot(Direction, LightDirectionReflected), 0.0), 128);
		fragColorCalc = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction)) + Specular;
		fragColor = vec4(fragColorCalc, 1.0);
	}
	else
	{
		Normal = -Normal;
		vec3 ReflectedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, reflect(Direction, Normal))).rgb;
		vec3 DirectionRefracted = refract(Direction, Normal, 1.332631);
		if (DirectionRefracted.x == 0.0 && DirectionRefracted.y == 0.0 && DirectionRefracted.z == 0.0)
		{
			fragColorCalc = ReflectedColor;
			fragColor = vec4(fragColorCalc, 1.0);
		}
		else
		{
			vec3 RefractedColor = textureCube(PoolSkyCubeMap, IntersectCubeMap(Position, DirectionRefracted)).rgb;
			fragColorCalc = mix(ReflectedColor, RefractedColor, -dot(Normal, Direction));
			fragColor = vec4(fragColorCalc, 1.0);
		}
	}
}
