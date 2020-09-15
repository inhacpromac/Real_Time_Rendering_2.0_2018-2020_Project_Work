#version 450 core

in vec3 transformed_normals;
in vec3 light_direction;
in vec3 viewer_vector;
in vec2 out_texture0_coord;
in vec4 Position;

in vec3 diffuse_color;

uniform vec3 u_La;
uniform vec3 u_Ld;
uniform vec3 u_Ls;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;

uniform float u_material_shininess;
uniform int u_lighting_enabled;
uniform sampler2D u_texture0_sampler;

out vec4 FragColor; 

void main(void)
{
	vec3 phong_ads_color;
	
	if (u_lighting_enabled == 1)
	{
		vec3 normalized_transformed_normals = normalize(transformed_normals);
		vec3 normalized_light_direction = normalize(light_direction);
		vec3 normalized_viewer_vector = normalize(viewer_vector);
		vec3 ambient = u_La * u_Ka;
		float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);
		vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;
		vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);
		vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);
		phong_ads_color = ambient + diffuse + specular + diffuse_color;
	}
	else
	{
		phong_ads_color        = vec3(1.0, 1.0, 1.0);
	}

	FragColor = /*texture(u_texture0_sampler, out_texture0_coord) */ vec4(phong_ads_color, 1.0);
	//FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
