//Header Files
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <GL/glew.h> // IMPORTANT : THIS FILE MUST BE INCLUDED BEFORE <gl/GL.h >  & <gl/GLU.h>
#include <gl/GL.h>
#include <math.h>

#include "uf/utilheader.h"
#include "uf/loadshader.h"
#include "vmath.h"

//#include "oglpp.h"

//#include "uf/mesh.h"


#include "uf/sphere.h"

#define NUMBER_OF_PARTICLES			50
#define NUMBER_OF_SPHERE			1
#define	NUMBER_OF_STEPS				10000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////BP
float transX[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];
float transY[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];
float transZ[NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE][NUMBER_OF_STEPS];

//BulletPhysics
#include "uf/bulletPhyHelloWorld.h"

using namespace vmath;

GLuint vs;
GLuint fs;
GLuint program;

GLuint vao_point_bp;
GLuint vbo_point_bp;

//mat4 PerspectiveProjectionMatrix;
extern mat4 perspectiveProjectionMatrix;
extern int giWidth;
extern int giHeight;
extern bool gbWaterDropDropped;
extern bool gbShowCubeMap;
extern bool gbShowLight;

GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;

GLuint L_KeyPressed_uniform;

GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint light_position_uniform;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

GLuint time_uniform;

GLuint sinUpdater_uniform;

mat4 gperspectiveProjectionMatrix;

GLfloat lightAmbient[]  		= { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[]  		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] 		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] 		= { 50.0f, 50.0f, 50.0f, 1.0f };

GLfloat material_ambient[] 		= { 0.05f, 0.05f, 0.0f, 1.0f };
GLfloat material_diffuse[] 		= { 0.5f, 0.5f, 0.4f, 1.0f };
GLfloat material_specular[] 	= { 0.7f, 0.7f, 0.04f, 1.0f };
GLfloat material_shininess 		= 0.078125 * 128.0f;

GLuint texture_sampler_unirom;
GLuint texture;
GLfloat timeValue;

extern FILE *gpLogFile;
extern bool gbAnimation;
float pointSizeVal = 2.0f;
float sinUpdaterVal = 10.0f;
bool sinUpdaterValchecker = false;
bool sinUpdaterValcheckerFalser = false;
int countTri = 0;
int countDrop = 0;
int iAddPos = 0;
int iAddPosDrop = 0;
float speed = 0.0f;

extern float cameraposX, cameraposY, cameraposZ;
extern float cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ;
extern float cameraUpX, cameraUpY, cameraUpZ;

int initializeDrop(void)
{
	// *** SHADER BLOCK ***
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader.vs", GL_VERTEX_SHADER);
	if (!vs)
	{
		
		exit(0);
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		exit(0);
	}

	// *** SHADER PROGRAM ***
	// create 
	program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// prelinking binding to vertex attributes //
	glBindAttribLocation(program, AMC_ATTRIBUTE_POSITION,  "vPosition");
	glBindAttribLocation(program, AMC_ATTRIBUTE_NORMALS,   "vNormal");
	glBindAttribLocation(program, AMC_ATTRIBUTE_TEXCOORD0, "vTexture0_Coord");


	// Link the shader program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(program, iInfoLogLength, &written, szInfoLog);
				fprintf(gpLogFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				
				DestroyWindow(NULL);
				exit(0);
			}
		}
	}

	glDetachShader(program, vs);
	glDeleteShader(vs);
	//vs = 0;
	glDetachShader(program, fs);
	glDeleteShader(fs);
	//fs = 0;

	// postlinking receiving uniform locations //
	model_matrix_uniform 	  	=  glGetUniformLocation(program, "u_model_matrix");
	view_matrix_uniform  	  	=  glGetUniformLocation(program, "u_view_matrix");
	projection_matrix_uniform 	=  glGetUniformLocation(program, "u_projection_matrix");
	//texture_sampler_unirom  	=  glGetUniformLocation(program, "u_texture0_sampler"); 

	L_KeyPressed_uniform      	=  glGetUniformLocation(program, "u_lighting_enabled");

	La_uniform				  	=  glGetUniformLocation(program, "u_La");
	Ld_uniform				  	=  glGetUniformLocation(program, "u_Ld");
	Ls_uniform				  	=  glGetUniformLocation(program, "u_Ls");

	light_position_uniform    	=  glGetUniformLocation(program, "u_light_position");


	Ka_uniform 				 	=  glGetUniformLocation(program, "u_Ka");
	Kd_uniform 				 	=  glGetUniformLocation(program, "u_Kd");
	Ks_uniform 				 	=  glGetUniformLocation(program, "u_Ks");

	material_shininess_uniform 	=  glGetUniformLocation(program, "u_material_shininess");

	time_uniform 			 	=  glGetUniformLocation(program, "time");


	sinUpdater_uniform 				= glGetUniformLocation(program, "sinUpdater");
	
	const GLfloat vertices[] = { 
										 0.0f,  0.0f, 0.0f,
										};

	// create vao_point_bp //
	glGenVertexArrays(1, &vao_point_bp);
	glBindVertexArray(vao_point_bp);
	glGenBuffers(1, &vbo_point_bp);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_point_bp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPointSize(4.0f);

	//btHelloWorldSim();


	float width    = 3.0f;
	float depth    = 3.0f;
	unsigned int m = 3;
	unsigned int n = 3;
	//generateMesh(width, depth, m, n);

	makeSphere(0.05, 50, 50);

	btHelloWorldInc();
	btHelloWorldSim();

	return(0);
}


int LoadGLTextures(GLuint *texture, TCHAR imageResourceId[])
{
	HBITMAP hBitmap;
	BITMAP bmp;
	int iStatus = FALSE;

	// code
	glGenTextures(1, texture);
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		iStatus = TRUE;
		GetObject(hBitmap, sizeof(bmp), &bmp);
		// configure image data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// sending image data to gpu
		glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGB,
				bmp.bmWidth,
				bmp.bmHeight,
				0,
				GL_BGR,
				GL_UNSIGNED_BYTE,
				bmp.bmBits);
		
		// create mipmaps for this texture for better image quality
		glGenerateMipmap(GL_TEXTURE_2D);
		
		DeleteObject(hBitmap); // delete unwanted bitmap handle
	}
	return (iStatus);
}



void displayDrop(void)
{
	//glViewport(0, 0, giWidth, giHeight);
	//code
	glPointSize(pointSizeVal);
	//glEnable(GL_DEPTH_TEST);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start Using shader program object //
	glUseProgram(program);

	//btHelloWorldSim();
	
	// declaration of matrices //
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 translationMatrix;
	mat4 scaleMatrix;
	mat4 rotationMatrix;

	viewMatrix = mat4::identity();

	//viewMatrix = lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(1.0f, 1.0f, 0.0f));
	static float iYScale = 1.2f;
	static bool bIsIncY = false;

	if (bIsIncY == false)
	{
		iYScale -= 0.005;
	}
	else
	{
		iYScale += 0.005;
	}

	if (iYScale >= 1.5f)
	{
		bIsIncY = false;
	}

	if (iYScale <= 1.2f)
	{
		bIsIncY = true;
	}

	viewMatrix = lookat(vec3(cameraposX, cameraposY, cameraposZ),
		vec3(cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ),
		vec3(cameraUpX, cameraUpY, cameraUpZ));

	for(countDrop = 0; countDrop < NUMBER_OF_PARTICLES + NUMBER_OF_SPHERE; countDrop++)
	{
		// initialization above matrices identity //
		modelMatrix 				= mat4::identity();
		viewMatrix 					= mat4::identity();
		translationMatrix 			= mat4::identity();

		//translationMatrix 			= translate(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), -20.0f);
		translationMatrix 			= translate(transX[countDrop][iAddPos], transY[countDrop][iAddPos], transZ[countDrop][iAddPos]);
		translationMatrix 			+= translate(0.0f, 0.0f, -2.7f);
		modelMatrix 				= translationMatrix;

		scaleMatrix 				= scale(1.0f, iYScale, 1.0f);
		modelMatrix *= scaleMatrix;
		
		glUniform1i(L_KeyPressed_uniform, 1);

		glUniform3fv(La_uniform, 1, lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);
		glUniform4fv(light_position_uniform, 1, lightPosition);

		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);

		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform1f(sinUpdater_uniform, sinUpdaterVal);

		// send to shader
		glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(view_matrix_uniform,  1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		

		// bind and draw
		if(countDrop == 0 && iAddPos <= 220)
		{
			sphereDraw();
			countDrop = 501;
		}
		
		glBindVertexArray(vao_point_bp);
		glDrawArrays(GL_POINTS, 0, 3);
		glBindVertexArray(0);
	}

	if (iAddPos > 150)
	{
		gbWaterDropDropped = true;
	}

	if (iAddPos > 250)
	{
		gbShowCubeMap = true;
	}

	if (iAddPos > 270)
	{
		gbShowLight = true;
	}

	if(iAddPos > 150)
	{
		for(countDrop = 1; countDrop < NUMBER_OF_PARTICLES; countDrop++)
		{
			// initialization above matrices identity //
			modelMatrix 				= mat4::identity();
			viewMatrix 					= mat4::identity();
			translationMatrix 			= mat4::identity();

			//translationMatrix 			= translate(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), -20.0f);
			translationMatrix 			= translate(transX[countDrop][iAddPos], transY[countDrop][iAddPos], transZ[countDrop][iAddPos]);
			translationMatrix			+= translate(0.0f, 0.0f, -2.7f);
			modelMatrix 				= translationMatrix;
			//modelMatrix *= viewMatrix;

			// send to shader
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(view_matrix_uniform,  1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			glBindVertexArray(vao_point_bp);
			glDrawArrays(GL_POINTS, 0, 3);
			glBindVertexArray(0);
		}
	}
	static bool bSinInc = false;
	if (sinUpdaterVal < -50.f)
	{
		bSinInc = true;
	}
	if (sinUpdaterVal > 50.f)
	{
		bSinInc = false;
	}
	if (bSinInc)
	{
		sinUpdaterVal += 1.0f;
	}
	else
	{
		sinUpdaterVal -= 1.0f;
	}
	/*if (sinUpdaterValcheckerFalser == false)
	{
		if(sinUpdaterVal < -50.0f)
			sinUpdaterVal += 1.0f;
		else if(sinUpdaterVal >= 50.0f)
		{
			sinUpdaterValchecker = true;
			sinUpdaterValcheckerFalser = true;
		}
	}

	if (sinUpdaterValchecker == true)
	{
		if(sinUpdaterVal >= 50.0f)
			sinUpdaterVal -= 1.0f;
		else if(sinUpdaterVal <= -50.0f)
		{
			sinUpdaterValchecker = false;
			sinUpdaterValcheckerFalser = false;
		}
	}*/
	
	glUseProgram(0);

	if (gbAnimation == true)
	{
	//	if (iAddPos >= 2000)
			//iAddPos = 0;
		//else
		{
			/*if (speed > 1.0)
			{
				speed = 0.0f;
			 	iAddPos += 1;
			}
			else
			{
				speed += 1.0;
			}*/
			iAddPos += 1;
		}
			
		// if (iAddPos > 500)
		// {
		// 	if(iAddPosDrop >= 2000)
		// 		iAddPosDrop = 0;
		// 	else
		// 		iAddPosDrop += 1;
		// }
	}
	timeValue += 0.05f;
}

