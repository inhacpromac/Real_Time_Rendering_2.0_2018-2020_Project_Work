#version 450 core

out vec4 fragColor;
uniform sampler2D u_sampler;
in vec4 starColor;
void main(void)
{
	//fragColor = starColor * texture(u_sampler, gl_PointCoord);
	fragColor = texture(u_sampler, gl_PointCoord) * vec4(1.0, 1.0, 1.0, 1.0/*0.5*/);
	
}
