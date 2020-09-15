using namespace vmath;

//Macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600


enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLORS,
	AMC_ATTRIBUTE_NORMALS,
	AMC_ATTRIBUTE_TEXCOORD0
};


FILE *gpFile = NULL;
FILE *objlog = NULL;


static GLfloat rAngle 	 = 0.0f;


mat4 PerspectiveProjectionMatrix;
mat4 viewMatrix;


int WindowWidth;
int WindowHeight;


GLfloat temp  = 0.0f;

GLfloat fps;
GLfloat mspf;

struct Light
{
	vec3 lightpos;
	vec4 ambientlight;
	vec4 diffuselight;
	vec4 specularlight;
	vec4 ambientmaterial;
	vec4 diffusematerial;
	vec4 specularmaterial;
	GLfloat shininess;
};
