#pragma once
#include "../../uf/sphere.h"
#include "room_scene0_water_wave.h"
#include "room_scene0_bullet_phy_water_drop.h"

bool bInit = false;
bool bAddDrop = false;
bool bLast = false;
bool bShowWater = false;
bool gbWaterDropDropped = false;
bool gbShowCubeMap = false;
bool gbShowLight = false;
float gfTransZEvery = 0.0f;

GLuint gCubeMapTexture;

GLuint WaterAddDropProgram;
GLuint WaterHeightMapProgram;
GLuint WaterNormalMapProgram;
GLuint PoolSkyProgram;
GLuint WaterProgram;
GLuint WaterDropProgram;

GLuint PoolSkyVAO;
GLuint PoolSkyVBO;
GLuint WaterVAO;
GLuint WaterVBO;
GLuint WaterWaveFBO;
GLuint vaoWaterAddDropProgram;
GLuint vboWaterAddDropProgram;

GLuint vao_point_bp;
GLuint vbo_point_bp;

GLuint WaterHeightMaps[2];
GLuint WaterHeightMapIndex;
GLuint WaterNormalMap;

GLuint WaterAddDropProgram_DropRadius_Uniform;
GLuint WaterAddDropProgram_Position_Uniform;
GLuint WaterAddDropProgram_WaterHeightMap_Uniform;
GLuint WaterProgram_CameraPosition_Uniform;
GLuint mvpUniformPoolSkyProgram;
GLuint mvpUniformWaterProgram;
GLuint WaterProgram_LightPosition_Uniform;

int QuadsVerticesCount;

// float cameraposX = 0.0f, cameraposY = 0.0f, cameraposZ = 1.0f;
// float cameraLooingPosX = 0.0f, cameraLooingPosY = 0.0, cameraLooingPosZ = 0.0f;
// float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f;

float cameraposX = 0.0f, cameraposY = 0.7f, cameraposZ = 1.4f;
float cameraLooingPosX = 0.0f, cameraLooingPosY = 0.0, cameraLooingPosZ = -1.0f;
float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f;

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

//GLuint time_uniform;

GLuint sinUpdater_uniform;

mat4 gperspectiveProjectionMatrix;

GLfloat lightAmbient[]  		= { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[]  		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] 		= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] 		= { 0.0f, -10.0f, -2.7f, 1.0f };

GLfloat material_ambient[] 		= { 0.28235f, 0.57254f, 0.60784f, 1.0f };
GLfloat material_diffuse[] 		= { 0.00784f, 0.02745f, 0.36470f, 1.0f};
GLfloat material_specular[] 	= { 0.28235f, 0.57254f, 0.60784f, 1.0f };
GLfloat material_shininess 		= 8.0f;

GLuint texture_sampler_unirom;
GLuint texture;

float pointSizeVal = 2.0f;
float sinUpdaterVal = 10.0f;
bool sinUpdaterValchecker = false;
bool sinUpdaterValcheckerFalser = false;
int countTri = 0;
int countDrop = 0;
int iAddPos = 0;
int iAddPosDrop = 0;
float speed = 0.0f;


extern int giWidth;
extern int giHeight;

GLint PrepareRoomScene0(void)
{
    int initializeDrop();
    int InitializeWaterWave();
    
    initializeDrop();
	if(InitializeWaterWave() == -1)
	{
		return -1;
	}

	return 0;
}

bool Pause = false;

void RenderRoomScene0(mat4 ProjectionMatrix, GLfloat time)
{
    void displayDrop(mat4);
    void AddDrop(float, float, float);
	
	static bool bDrawDropOne = false;
	static bool bDrawDropTwo = false;
	vec3 LightPosition = vec3(0.0f, 5.5f, -9.5f);
	//if (gbWaterDropDropped)
	{
		if (!Pause)
		{
			static DWORD LastTime = GetTickCount();

			DWORD Time = GetTickCount();

		}

		if (bInit)
		{
			cameraposX = 0.0f;
			cameraposY = 0.7f;
			cameraposZ = 1.4f;

			cameraLooingPosX = 0.0f;
			cameraLooingPosY = 0.0f;
			cameraLooingPosZ = -1.0f;

			cameraUpX = 0.0f;
			cameraUpY = 1.0f;
			cameraUpZ = 0.0f;
		}
		//if (bAddDrop)
		if(gbWaterDropDropped && bDrawDropOne == false)
		{
			cameraposX = 0.0f;
			cameraposY = 0.7f;
			cameraposZ = 1.4f;

			cameraLooingPosX = 0.0f;
			cameraLooingPosY = 0.0f;
			cameraLooingPosZ = -1.0f;

			cameraUpX = 0.0f;
			cameraUpY = 1.0f;
			cameraUpZ = 0.0f;
			//fprintf(gpFile, "\nErrorrrrr\n");
			AddDrop(0.0, 0.2, 4.0f / 128.0f);
			//fprintf(gpFile, "\nErrorrrrrAfter\n");
			bAddDrop = false;
			bDrawDropOne = true;
		}
		//AddDrop(0.0, 0.2, 4.0f / 128.0f);
		if (bDrawDropOne == true && bDrawDropTwo == false)
		{
			AddDrop(0.0, 0.2, 4.0f / 128.0f);
			bDrawDropTwo = true;
		}
		if (bLast)
		{
			cameraposX = 0.0f;
			cameraposY = 2.5f;
			cameraposZ = -1.0f;

			cameraLooingPosX = 0.0f;
			cameraLooingPosY = 0.0f;
			cameraLooingPosZ = -1.0f;

			cameraUpX = 0.0f;
			cameraUpY = 0.0f;
			cameraUpZ = -1.0f;
		}


		// update water surface ---------------------------------------------------------------------------------------------------

		static DWORD LastTime = GetTickCount();

		DWORD Time = GetTickCount();

		if (Time - LastTime >= 16)
		{
			LastTime = Time;

			// update water height map --------------------------------------------------------------------------------------------

			glViewport(0, 0, WATER_HEIGTH_MAP_RESOLUTION, WATER_HEIGTH_MAP_RESOLUTION);


			GLuint WHMID = (WaterHeightMapIndex + 1) % 2;

			glBindFramebuffer(GL_FRAMEBUFFER, WaterWaveFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, WaterHeightMaps[WHMID], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

			glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WaterHeightMapIndex]);
			glUseProgram(WaterHeightMapProgram);
			glBindVertexArray(vaoWaterAddDropProgram);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glBindVertexArray(0);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WHMID]);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			++WaterHeightMapIndex %= 2;

			// update water normal map --------------------------------------------------------------------------------------------

			glViewport(0, 0, WATER_NORMAL_MAP_RESOLUTION, WATER_NORMAL_MAP_RESOLUTION);

			glBindFramebuffer(GL_FRAMEBUFFER, WaterWaveFBO);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, WaterNormalMap, 0);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

			glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WaterHeightMapIndex]);
			glUseProgram(WaterNormalMapProgram);
			glBindVertexArray(vaoWaterAddDropProgram);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glBindVertexArray(0);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, WaterNormalMap);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// render pool sky mesh ---------------------------------------------------------------------------------------------------

		glViewport(0, 0, giWidth, giHeight);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);




		//if (WireFrame)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// declaration of metrices
		mat4 modelMatrix;
		mat4 viewMatrix;
		mat4 projectionMatrix;
		mat4 modelViewProjectionMatrix;


		// initializing above metrices to identity
		modelMatrix = mat4::identity();
		viewMatrix = mat4::identity();
		projectionMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		//modelMatrix

		viewMatrix = lookat(vec3(cameraposX, cameraposY, cameraposZ),
			vec3(cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ),
			vec3(cameraUpX, cameraUpY, cameraUpZ));

		modelMatrix = translate(0.0f, -0.5f, gfTransZEvery);
		projectionMatrix = ProjectionMatrix;


		// do necessary matrix multiplication
		modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;


		glBindTexture(GL_TEXTURE_CUBE_MAP, gCubeMapTexture);
		glUseProgram(PoolSkyProgram);
		glBindVertexArray(PoolSkyVAO);
		glUniformMatrix4fv(mvpUniformPoolSkyProgram, 1, GL_FALSE, modelViewProjectionMatrix);
		//glBindBuffer(GL_ARRAY_BUFFER, PoolSkyVBO);
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(3, GL_FLOAT, 12, (void*)0);
		//glDrawArrays(GL_QUADS, 0, 24);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
		//glDisableClientState(GL_VERTEX_ARRAY);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


		glDisable(GL_CULL_FACE);

		// render water surface ---------------------------------------------------------------------------------------------------


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WaterHeightMapIndex]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, WaterNormalMap);

		
		if (gbShowCubeMap)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, gCubeMapTexture);
		}

		glUseProgram(WaterProgram);
		
		if(gbShowLight)
		{
			//glUniform3fv(WaterProgram_LightPosition_Uniform, 1, LightPosition);
		}
		modelMatrix = translate(0.0f, 0.0f, gfTransZEvery);
		modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		vec3 cameraPosition = vec3(cameraposX, cameraposY, cameraposZ);
		glUniform3fv(WaterProgram_CameraPosition_Uniform, 1, cameraPosition);
		glUniformMatrix4fv(mvpUniformWaterProgram, 1, GL_FALSE, modelViewProjectionMatrix);
		//glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(3, GL_FLOAT, 12, (void*)0);
		//glDrawArrays(GL_QUADS, 0, QuadsVerticesCount);
		//glDisableClientState(GL_VERTEX_ARRAY);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(WaterVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, QuadsVerticesCount);
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

		/*if (WireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}*/
	}
	//else
	{
		displayDrop(ProjectionMatrix);
	}
	
	glDisable(GL_DEPTH_TEST);

	if (gbShowCubeMap)
	{
		if (cameraposY <= 2.5f)
		{
			cameraposY += 0.001f;
		}
		if (cameraposZ >= -1.0f)
		{
			cameraposZ -= 0.01f;
		}

		if (cameraUpY >= 0.0f)
		{
			cameraUpY -= 0.01f;
		}
		
		if (cameraUpZ >= -1.0f)
		{
			cameraUpZ -= 0.01f;
		}

		if (gfTransZEvery > -0.35f)
		{
			gfTransZEvery -= 0.01f;
		}
	}
}

void DeallocateRoomScene0(void)
{

}

bool LoadBmpTexture(BITMAP *bmp, TCHAR imageResourceID[])
{

	HBITMAP hBitmap;
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		imageResourceID,
		IMAGE_BITMAP,
		0,
		0,
		LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		GetObject(hBitmap, sizeof(BITMAP), bmp);
		return true;
	}

	return false;
}

int InitializeWaterWave()
{
	//CubeMap
	bool Error = false;
	bool LoadBmpTexture(BITMAP *bmp, TCHAR imageResourceID[]);

	glGenTextures(1, &gCubeMapTexture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, gCubeMapTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	bool bRet = false;

	for (int i = 0; i < 6; i++)
	{
		BITMAP bmp;
		bRet = false;
		bRet = LoadBmpTexture(&bmp, MAKEINTRESOURCE(CUBE_MAP_RIGHT + i));
		if (bRet == true)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
		}
		else
		{
			fprintf_s(gpFile, "LoadBmpTexture() failed at i = %d \n", i);
			break;
		}
	}

	if (bRet == false)
	{
		return -1;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	int LoadWaterAddDropProgram();
	int LoadWaterHeightMapProgram();
	int LoadWaterNormalMapProgram();
	int LoadPoolSkyProgram();
	int LoadWaterProgram();

	if(LoadWaterAddDropProgram() == -1)
	{
		return -1;
	}

	if(LoadWaterHeightMapProgram() == -1)
	{
		return -1;
	}

	if(LoadWaterNormalMapProgram() == -1)
	{
		return -1;
	}

	if(LoadPoolSkyProgram() == -1)
	{
		return -1;
	}

	if(LoadWaterProgram() == -1)
	{
		return -1;
	}

	const GLfloat WaterAddDropProgramVertices[] =
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// create vao
	glGenVertexArrays(1, &vaoWaterAddDropProgram);
	glBindVertexArray(vaoWaterAddDropProgram);
	glGenBuffers(1, &vboWaterAddDropProgram);
	glBindBuffer(GL_ARRAY_BUFFER, vboWaterAddDropProgram);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WaterAddDropProgramVertices), WaterAddDropProgramVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(2, WaterHeightMaps);

	vec4 *Heights = new vec4[WATER_HEIGTH_MAP_RESOLUTION * WATER_HEIGTH_MAP_RESOLUTION];

	for (int i = 0; i < WATER_HEIGTH_MAP_RESOLUTION * WATER_HEIGTH_MAP_RESOLUTION; i++)
	{
		Heights[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WATER_HEIGTH_MAP_RESOLUTION, WATER_HEIGTH_MAP_RESOLUTION, 0, GL_RGBA, GL_FLOAT, Heights);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	delete[] Heights;


	glGenTextures(1, &WaterNormalMap);

	vec4 *Normals = new vec4[WATER_NORMAL_MAP_RESOLUTION * WATER_NORMAL_MAP_RESOLUTION];

	for (int i = 0; i < WATER_NORMAL_MAP_RESOLUTION * WATER_NORMAL_MAP_RESOLUTION; i++)
	{
		Normals[i] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	glBindTexture(GL_TEXTURE_2D, WaterNormalMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WATER_NORMAL_MAP_RESOLUTION, WATER_NORMAL_MAP_RESOLUTION, 0, GL_RGBA, GL_FLOAT, Normals);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] Normals;


	glGenVertexArrays(1, &PoolSkyVAO);
	glBindVertexArray(PoolSkyVAO);
	glGenBuffers(1, &PoolSkyVBO);
	float PoolSkyVertices[] =
	{	// x, y, z, x, y, z, x, y, z, x, y, z
		 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // +X
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, // -X
		-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, // +Y
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // -Y
		 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, // +Z
		-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f  // -Z
	};

	glBindBuffer(GL_ARRAY_BUFFER, PoolSkyVBO);
	glBufferData(GL_ARRAY_BUFFER, 288, PoolSkyVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glGenVertexArrays(1, &WaterVAO);
	glBindVertexArray(WaterVAO);
	glGenBuffers(1, &WaterVBO);

	int WMRP1 = WATER_MAP_RESOLUTION + 1;

	vec3 *Vertices = new vec3[WMRP1 * WMRP1];

	float WMSDWMR = 2.0f / (float)WATER_MAP_RESOLUTION;

	for (int y = 0; y <= WATER_MAP_RESOLUTION; y++)
	{
		for (int x = 0; x <= WATER_MAP_RESOLUTION; x++)
		{
			Vertices[WMRP1 * y + x][0] = x * WMSDWMR - 1.0f;
			Vertices[WMRP1 * y + x][1] = 0.0f;
			Vertices[WMRP1 * y + x][2] = 1.0f - y * WMSDWMR;
		}
	}

	CBuffer Quads;

	for (int y = 0; y < WATER_MAP_RESOLUTION; y++)
	{
		int yp1 = y + 1;

		for (int x = 0; x < WATER_MAP_RESOLUTION; x++)
		{
			int xp1 = x + 1;

			int a = WMRP1 * y + x;
			int b = WMRP1 * y + xp1;
			int c = WMRP1 * yp1 + xp1;
			int d = WMRP1 * yp1 + x;

			Quads.AddData(&Vertices[a], 12);
			Quads.AddData(&Vertices[b], 12);
			Quads.AddData(&Vertices[c], 12);
			Quads.AddData(&Vertices[d], 12);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, WaterVBO);
	glBufferData(GL_ARRAY_BUFFER, Quads.GetDataSize(), Quads.GetData(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	QuadsVerticesCount = Quads.GetDataSize() / 12;

	Quads.Empty();

	delete[] Vertices;

	glGenFramebuffers(1, &WaterWaveFBO);

	srand(GetTickCount());

	return 0;
}

int LoadWaterAddDropProgram()
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water_add_drop.vs", GL_VERTEX_SHADER);
	if (!vs)
	{	
		return -1;
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water_add_drop.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		return -1;
	}

	// create shader program object
	WaterAddDropProgram = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(WaterAddDropProgram, vs);

	// attach fragment shader to shader program
	glAttachShader(WaterAddDropProgram, fs);

	// prelinking binding to vertex attributes
	glBindAttribLocation(WaterAddDropProgram, AMC_ATTRIBUTE_POSITION, "vPosition");

	// link the shader program
	glLinkProgram(WaterAddDropProgram);

	glGetProgramiv(WaterAddDropProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(WaterAddDropProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(WaterAddDropProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	WaterAddDropProgram_DropRadius_Uniform = glGetUniformLocation(WaterAddDropProgram, "DropRadius");
	WaterAddDropProgram_Position_Uniform = glGetUniformLocation(WaterAddDropProgram, "Position");
	WaterAddDropProgram_WaterHeightMap_Uniform = glGetUniformLocation(WaterAddDropProgram, "WaterHeightMap");

	return 0;
}

int LoadWaterHeightMapProgram()
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water_height_map.vs", GL_VERTEX_SHADER);
	if (!vs)
	{	
		return -1;
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water_height_map.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		return -1;
	}

	// create shader program object
	WaterHeightMapProgram = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(WaterHeightMapProgram, vs);

	// attach fragment shader to shader program
	glAttachShader(WaterHeightMapProgram, fs);

	// prelinking binding to vertex attributes
	glBindAttribLocation(WaterHeightMapProgram, AMC_ATTRIBUTE_POSITION, "vPosition");

	// link the shader program
	glLinkProgram(WaterHeightMapProgram);

	glGetProgramiv(WaterHeightMapProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(WaterHeightMapProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(WaterHeightMapProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	glUseProgram(WaterHeightMapProgram);
	glUniform1f(glGetUniformLocation(WaterHeightMapProgram, "ODWHMR"), 1.0f / (float)WATER_HEIGTH_MAP_RESOLUTION);
	glUniform1i(glGetUniformLocation(WaterHeightMapProgram, "WaterHeightMap"), 0);
	glUseProgram(0);

	return 0;
}

int LoadWaterNormalMapProgram()
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water_normal_map.vs", GL_VERTEX_SHADER);
	if (!vs)
	{	
		return -1;
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water_normal_map.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		return -1;
	}

	// create shader program object
	WaterNormalMapProgram = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(WaterNormalMapProgram, vs);

	// attach fragment shader to shader program
	glAttachShader(WaterNormalMapProgram, fs);

	// prelinking binding to vertex attributes
	glBindAttribLocation(WaterNormalMapProgram, AMC_ATTRIBUTE_POSITION, "vPosition");

	// link the shader program
	glLinkProgram(WaterNormalMapProgram);

	glGetProgramiv(WaterNormalMapProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(WaterNormalMapProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(WaterNormalMapProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	glUseProgram(WaterNormalMapProgram);
	glUniform1f(glGetUniformLocation(WaterNormalMapProgram, "ODWNMR"), 1.0f / (float)WATER_NORMAL_MAP_RESOLUTION); // Will be used to calculate normal for HeightMap 
	glUniform1f(glGetUniformLocation(WaterNormalMapProgram, "TDWNMR"), 2.0f / (float)WATER_NORMAL_MAP_RESOLUTION); // Will be used for normalizing the values
	glUniform1i(glGetUniformLocation(WaterNormalMapProgram, "WaterHeightMap"), 0);
	glUseProgram(0);

	return 0;
}

int LoadPoolSkyProgram()
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water_pool_sky.vs", GL_VERTEX_SHADER);
	if (!vs)
	{	
		return -1;
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water_pool_sky.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		return -1;
	}

	// create shader program object
	PoolSkyProgram = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(PoolSkyProgram, vs);

	// attach fragment shader to shader program
	glAttachShader(PoolSkyProgram, fs);

	// prelinking binding to vertex attributes
	glBindAttribLocation(PoolSkyProgram, AMC_ATTRIBUTE_POSITION, "vPosition");	

	// link the shader program
	glLinkProgram(PoolSkyProgram);

	glGetProgramiv(PoolSkyProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(PoolSkyProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(PoolSkyProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	// post linking retreving uniform location
	mvpUniformPoolSkyProgram = glGetUniformLocation(PoolSkyProgram, "u_mvp_matrix");

	glUseProgram(PoolSkyProgram);
	glUniform1i(glGetUniformLocation(PoolSkyProgram, "PoolSkyCubeMap"), 0);
	glUseProgram(0);
	
	return 0;
}

int LoadWaterProgram()
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water.vs", GL_VERTEX_SHADER);
	if (!vs)
	{	
		return -1;
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		return -1;
	}

	// create shader program object
	WaterProgram = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(WaterProgram, vs);

	// attach fragment shader to shader program
	glAttachShader(WaterProgram, fs);

	// prelinking binding to vertex attributes
	glBindAttribLocation(WaterProgram, AMC_ATTRIBUTE_POSITION, "vPosition");

	// link the shader program
	glLinkProgram(WaterProgram);

	glGetProgramiv(WaterProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(WaterProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(WaterProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	// post linking retreving uniform location
	mvpUniformWaterProgram = glGetUniformLocation(WaterProgram, "u_mvp_matrix");

	vec3 CubeMapNormals[6] = {
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, 1.0f),
	};

	glUseProgram(WaterProgram);
	glUniform1i(glGetUniformLocation(WaterProgram, "WaterHeightMap"), 0);
	glUniform1i(glGetUniformLocation(WaterProgram, "WaterNormalMap"), 1);
	glUniform1i(glGetUniformLocation(WaterProgram, "PoolSkyCubeMap"), 2);
	WaterProgram_LightPosition_Uniform = glGetUniformLocation(WaterProgram, "LightPosition");
	WaterProgram_CameraPosition_Uniform = glGetUniformLocation(WaterProgram, "CameraPosition");
	glUniform3fv(glGetUniformLocation(WaterProgram, "CubeMapNormals"), 6, (float*)CubeMapNormals);
	glUseProgram(0);
	
	return 0;
}

void AddDrop(float x, float y, float DropRadius)
{
	if (x >= -1.0f && x <= 1.0f && y >= -1.0f && y <= 1.0f)
	{
		glViewport(0, 0, WATER_MAP_RESOLUTION, WATER_MAP_RESOLUTION);

		glBindFramebuffer(GL_FRAMEBUFFER, WaterWaveFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, WaterHeightMaps[(WaterHeightMapIndex + 1) % 2], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

		glBindTexture(GL_TEXTURE_2D, WaterHeightMaps[WaterHeightMapIndex]);
		glUseProgram(WaterAddDropProgram);
		glUniform1i(WaterAddDropProgram_WaterHeightMap_Uniform, 0);
		glUniform1f(WaterAddDropProgram_DropRadius_Uniform, DropRadius);
		vec2 temp = vec2(x * 0.5f + 0.5f, 0.5f - y * 0.5f);
		glUniform2fv(WaterAddDropProgram_Position_Uniform, 1, temp);
		glBindVertexArray(vaoWaterAddDropProgram);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		++WaterHeightMapIndex %= 2;
	}
}

int initializeDrop(void)
{
	GLuint vs;
	GLuint fs;
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// *** VERTEX SHADER ***
	vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_water_drop_bp.vs", GL_VERTEX_SHADER);
	if (!vs)
	{
		
		exit(0);
	}

	// *** FRAGMENT SHADER ***
	fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_water_drop_bp.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	{
		exit(0);
	}

	// *** SHADER PROGRAM ***
	// create 
	WaterDropProgram = glCreateProgram();

	glAttachShader(WaterDropProgram, vs);
	glAttachShader(WaterDropProgram, fs);

	// prelinking binding to vertex attributes //
	glBindAttribLocation(WaterDropProgram, AMC_ATTRIBUTE_POSITION,  "vPosition");
	glBindAttribLocation(WaterDropProgram, AMC_ATTRIBUTE_NORMALS,   "vNormal");
	glBindAttribLocation(WaterDropProgram, AMC_ATTRIBUTE_TEXCOORD0, "vTexture0_Coord");


	// Link the shader WaterDropProgram
	glLinkProgram(WaterDropProgram);
	glGetProgramiv(WaterDropProgram, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(WaterDropProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(WaterDropProgram, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				
				DestroyWindow(NULL);
				exit(0);
			}
		}
	}

	glDetachShader(WaterDropProgram, vs);
	glDeleteShader(vs);
	//vs = 0;
	glDetachShader(WaterDropProgram, fs);
	glDeleteShader(fs);
	//fs = 0;

	// postlinking receiving uniform locations //
	model_matrix_uniform 	  	=  glGetUniformLocation(WaterDropProgram, "u_model_matrix");
	view_matrix_uniform  	  	=  glGetUniformLocation(WaterDropProgram, "u_view_matrix");
	projection_matrix_uniform 	=  glGetUniformLocation(WaterDropProgram, "u_projection_matrix");
	//texture_sampler_unirom  	=  glGetUniformLocation(WaterDropProgram, "u_texture0_sampler"); 

	L_KeyPressed_uniform      	=  glGetUniformLocation(WaterDropProgram, "u_lighting_enabled");

	La_uniform				  	=  glGetUniformLocation(WaterDropProgram, "u_La");
	Ld_uniform				  	=  glGetUniformLocation(WaterDropProgram, "u_Ld");
	Ls_uniform				  	=  glGetUniformLocation(WaterDropProgram, "u_Ls");

	light_position_uniform    	=  glGetUniformLocation(WaterDropProgram, "u_light_position");


	Ka_uniform 				 	=  glGetUniformLocation(WaterDropProgram, "u_Ka");
	Kd_uniform 				 	=  glGetUniformLocation(WaterDropProgram, "u_Kd");
	Ks_uniform 				 	=  glGetUniformLocation(WaterDropProgram, "u_Ks");

	material_shininess_uniform 	=  glGetUniformLocation(WaterDropProgram, "u_material_shininess");

	//time_uniform 			 	=  glGetUniformLocation(WaterDropProgram, "time");


	sinUpdater_uniform 				= glGetUniformLocation(WaterDropProgram, "sinUpdater");
	
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

void displayDrop(mat4 perspectiveProjectionMatrix)
{
	//glViewport(0, 0, giWidth, giHeight);
	//code
	glPointSize(pointSizeVal);
	//glEnable(GL_DEPTH_TEST);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start Using shader WaterDropProgram object //
	glUseProgram(WaterDropProgram);

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

	if (iYScale >= 1.3f)
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

	// if (gbAnimation == true)
	// {
	// //	if (iAddPos >= 2000)
			// //iAddPos = 0;
		// //else
		// {
			// /*if (speed > 1.0)
			// {
				// speed = 0.0f;
			 	// iAddPos += 1;
			// }
			// else
			// {
				// speed += 1.0;
			// }*/
			// iAddPos += 1;
		// }
			
		// // if (iAddPos > 500)
		// // {
		// // 	if(iAddPosDrop >= 2000)
		// // 		iAddPosDrop = 0;
		// // 	else
		// // 		iAddPosDrop += 1;
		// // }
	// }
	iAddPos += 1;
	//timeValue += 0.05f;
}



