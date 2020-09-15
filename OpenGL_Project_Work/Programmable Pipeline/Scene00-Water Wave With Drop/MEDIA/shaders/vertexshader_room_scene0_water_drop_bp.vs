#version 430 core

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexture0_Coord;

uniform mat4  u_model_matrix;
uniform mat4  u_view_matrix;
uniform mat4  u_projection_matrix;
uniform vec4  u_light_position;
uniform int   u_lighting_enabled;
uniform float time;

uniform float sinUpdater;

out vec3 transformed_normals;
out vec3 light_direction;
out vec3 viewer_vector;
out vec2 out_texture0_coord;
out vec4 Position;

out vec3 diffuse_color;

const float amplitude   = 0.125;
const float frequency   = 4;
const float PI          = 3.14159;

void main(void)
{
    vec3 u_diffuse_light_position = vec3(0.0, 0.0, 0.0);
    vec3 u_kd_diffuse_material = vec3(0.00784, 0.02745, 0.36470);
    vec3 u_ld_diffuse_light = vec3(1.0, 1.0, 1.0);
    if (u_lighting_enabled == 1)
    {
        vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;
        transformed_normals  = mat3(u_view_matrix * u_model_matrix) * vNormal;
        light_direction      = vec3(u_light_position) - eye_coordinates.xyz;
        viewer_vector        = -eye_coordinates.xyz;

        mat3 normal_matrix = mat3(transpose(inverse(u_view_matrix * u_model_matrix)));
        vec3 tNorm = normalize(normal_matrix * vNormal);
        vec3 s = normalize(vec3(u_diffuse_light_position - eye_coordinates.xyz));
        diffuse_color = u_ld_diffuse_light * u_kd_diffuse_material * max(dot(s, tNorm), 0.0);
    }

    float distance = length(vPosition);
    float y        = amplitude * sin(-PI*distance*frequency+time);

    //gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(vPosition.x, y, vPosition.z, 1.0);
    vec4 Pos = vPosition;
    /*if (sinUpdater == 10.0)
        Pos.x  = Pos.x + sin(Pos.y * 10.0) * 0.1;
    else if (sinUpdater == 20.0)
        Pos.x  = Pos.x + sin(Pos.y * 20.0) * 0.1;
    else if (sinUpdater == 50.0)
        Pos.x  = Pos.x + sin(Pos.y * 50.0) * 0.1;
    else if (sinUpdater == 70.0)
        Pos.x  = Pos.x + sin(Pos.y * 70.0) * 0.1;
    else if (sinUpdater == 80.0)*/
        //Pos.y  = Pos.y + sin(Pos.x * sinUpdater) * 0.1;
		Pos.x  = Pos.x + sin(Pos.y * sinUpdater) * 0.01;
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * Pos;
    out_texture0_coord = vTexture0_Coord;
    //Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;
}
