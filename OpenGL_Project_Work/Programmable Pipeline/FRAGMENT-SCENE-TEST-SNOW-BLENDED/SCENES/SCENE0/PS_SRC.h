
GLuint vao_room_scene0_ps;
GLuint vbo_room_scene0_pos_ps;
GLuint program_room_scene0_ps;
GLuint vbo_room_scene0_pos_gpu_ps;

GLuint modelViewProjectionMatrixUinform_room_scene0_ps;
GLuint textureSamplerUniform_room_scene0_ps;
GLuint gPointSprite_ps;
GLuint u_time_uniform;

float position[MAX_MESH_WIDTH][MAX_MESH_HEIGHT][4] = {0}; // initializing to avoid zero out each
//float position_clr[MAX_MESH_WIDTH][MAX_MESH_HEIGHT][4] = { 0 }; // initializing to avoid zero out each
float position_direction[MAX_MESH_WIDTH][MAX_MESH_HEIGHT][3] = { 0 };
float position_gravity[MAX_MESH_WIDTH][MAX_MESH_HEIGHT][3] = { 0 };
float position_life[MAX_MESH_WIDTH][MAX_MESH_HEIGHT] = { 1.0f };
float position_fade[MAX_MESH_WIDTH][MAX_MESH_HEIGHT] = { (float)((rand() % 100) / 1000.0f) + 0.001f };
float gfSlowDown = 5.0f;

struct cudaGraphicsResource *gpGraphicsResource = NULL;
float4 *gpGpuPosition = { NULL };
float3 *gpGpuPosDir = { NULL };
float3 *gpGpuPosDirInitial = { NULL };
float3 *gpGpuPosGravity = { NULL };
float *gpGpuPosLife = { NULL };
float *gpGpuPosFade = { NULL };
float gGpuvelocity = 1.0;

cudaError_t cuda_error;

static bool bStartTime = true;
float gTransParticleOriginX_3rd = 0.0f;
float gTransParticleOriginX = 50.0f;
float gTransParticleOriginY = 7.0f;
float gTransParticleOriginZ = 4.5f;

extern float cameraposZ;

/*float cameraposX = 0.0f, cameraposY = 0.0f, cameraposZ = -10.0f;
float cameraLooingPosX = 0.0f, cameraLooingPosY = 0.0, cameraLooingPosZ = 0.0f;
float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f;*/

int gMeshWidth = 384;
int gMeshHeight = 384;

float gfAnimationTime = 0.0f;
float gfSize = 30.0f;

GLint PrepareRoomScene0_ps(void)
{
	GLint status = 0;

	// *** SHADER BLOCK ***
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	int iErrorIndex = 0;

	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//0
	fflush(gpFile);

	// *** VERTEX SHADER ***
	GLuint vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0_ps.vs", GL_VERTEX_SHADER);
	if (!vs)
    return -1;
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//1
	fflush(gpFile);

	// *** FRAGMENT SHADER ***
	GLuint fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0_ps.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	return -1;
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//2
	fflush(gpFile);

	// *** SHADER PROGRAM ***
	// create 
	program_room_scene0_ps = glCreateProgram();
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//3
	fflush(gpFile);

	glAttachShader(program_room_scene0_ps, vs);
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//4
	fflush(gpFile);
	glAttachShader(program_room_scene0_ps, fs);
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//5
	fflush(gpFile);

	// prelinking binding to vertex attribute //
	glBindAttribLocation(program_room_scene0_ps, AMC_ATTRIBUTE_POSITION,  "vPosition");
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//6
	fflush(gpFile);

	// Link the shader program_room_scene0
	glLinkProgram(program_room_scene0_ps);
	glGetProgramiv(program_room_scene0_ps, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(program_room_scene0_ps, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(program_room_scene0_ps, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	glDetachShader(program_room_scene0_ps, vs);
	glDeleteShader(vs);
	//vs = 0;
	glDetachShader(program_room_scene0_ps, fs);
	glDeleteShader(fs);
	//fs = 0;

	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//7
	fflush(gpFile);

	// postlinking receiving uniform location //
	modelViewProjectionMatrixUinform_room_scene0_ps 		= glGetUniformLocation(program_room_scene0_ps, "u_mvp_matrix");
	
	textureSamplerUniform_room_scene0_ps 	= glGetUniformLocation(program_room_scene0_ps, "u_sampler");
	u_time_uniform = glGetUniformLocation(program_room_scene0_ps, "time");

	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//8
	fflush(gpFile);

    for (int i = 0; i < MAX_MESH_WIDTH; i++)
	{
		for (int j = 0; j < MAX_MESH_HEIGHT; j++)
		{
			//position[i][j][1] = P_INIT_POS_Y;
			position_life[i][j] = 2.0f;
			position_fade[i][j] = (float)((rand() % 1000) / 1000.0f) + 0.001f;
			//position_direction[i][j][0] = (float)((rand() % 50) - 26.0f)*10.0f;
			//position_direction[i][j][1] = (float)((rand() % 50) - 25.0f)*10.0f;
			//position_direction[i][j][2] = (float)((rand() % 50) - 25.0f)*10.0f;

			for (int k = 0; k < 4; k++)
			{
				//position_clr[i][j][0] = 0.8f + random_float() * 0.2f;
				//position_clr[i][j][1] = 0.8f + random_float() * 0.2f;
				//position_clr[i][j][2] = 0.8f + random_float() * 0.2f;

				//position[i][j][k] = 0.0f;
				if (k == 0)
				{
					position_direction[i][j][k] = (float)((rand() % 50) - 25.0f)*10.0f;
					//position[i][j][k] += position_direction[i][j][k] / (gfSlowDown * 1000);
					position_gravity[i][j][k] = 0.0f;
				}
				if (k == 1)
				{
					position_direction[i][j][k] = (float)((rand() % 50) - 25.0f)*10.0f;
					//position[i][j][k] = 20;// += position_direction[i][j][k] / (gfSlowDown * 1000);
					position_gravity[i][j][k] = -0.8f;
					//position[i][j][k] = -10.0f;
				}
				if (k == 2)
				{
					position_direction[i][j][k] = (float)((rand() % 50) - 25.0f)*10.0f;
					//position[i][j][k] += position_direction[i][j][k] / (gfSlowDown * 1000);
					//position[i][j][k] = 0.0f;
					position_gravity[i][j][k] = 0.0f;
					//position[i][j][k] = -100;
				}
				if (k == 3)
				{
					position[i][j][k] = 1.0f;
				}
			}
			
		}
	}
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//9
	fflush(gpFile);

    // create vao
	glGenVertexArrays(1, &vao_room_scene0_ps);
	glBindVertexArray(vao_room_scene0_ps);
	glGenBuffers(1, &vbo_room_scene0_pos_ps);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_room_scene0_pos_ps);
	glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//10
	fflush(gpFile);
	/*glGenBuffers(1, &vboColor);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * 3, position_clr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);*/
	//free(position_clr);
	glGenBuffers(1, &vbo_room_scene0_pos_gpu_ps);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_room_scene0_pos_gpu_ps);
	glBufferData(GL_ARRAY_BUFFER, MY_ARRAY_SIZE * 4, NULL, GL_DYNAMIC_DRAW);
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//11
	fflush(gpFile);

	cuda_error = cudaGraphicsGLRegisterBuffer(&gpGraphicsResource, vbo_room_scene0_pos_gpu_ps, cudaGraphicsRegisterFlagsWriteDiscard);
	if (cuda_error != cudaSuccess)
	{
		return -6;
	}
	fprintf(gpFile, "PrepareRoomScene0_ps cudaGraphicsGLRegisterBuffer Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//12
	fflush(gpFile);


	/*glGenBuffers(1, &vboColor);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorVertices), colorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	cuda_error = cudaMalloc(&gpGpuPosition, MY_ARRAY_SIZE * 4);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosition) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//13
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosition, position, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosition) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//14
	fflush(gpFile);

	cuda_error = cudaMalloc(&gpGpuPosDir, MY_ARRAY_SIZE * 4);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosDir) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//15
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosDir, position_direction, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosDir) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//16
	fflush(gpFile);

	cuda_error = cudaMalloc(&gpGpuPosDirInitial, MY_ARRAY_SIZE * 4);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosDirInitial) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//17
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosDirInitial, position_direction, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosDirInitial) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//18
	fflush(gpFile);

	cuda_error = cudaMalloc(&gpGpuPosGravity, MY_ARRAY_SIZE * 4);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosGravity) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//19
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosGravity, position_gravity, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosGravity) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//20
	fflush(gpFile);

	cuda_error = cudaMalloc(&gpGpuPosFade, MAX_MESH_WIDTH * MAX_MESH_HEIGHT);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosFade) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//21
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosFade, position_fade, MAX_MESH_WIDTH * MAX_MESH_HEIGHT, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosFade) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//22
	fflush(gpFile);

	cuda_error = cudaMalloc(&gpGpuPosLife, MY_ARRAY_SIZE * 2);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMalloc(gpGpuPosLife) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//23
	fflush(gpFile);
	cuda_error = cudaMemcpy(gpGpuPosLife, position_life, MY_ARRAY_SIZE * 2, cudaMemcpyHostToDevice);
	fprintf(gpFile, "PrepareRoomScene0_ps cudaMemcpy(gpGpuPosLife) Error Index : %d : %s\n",iErrorIndex++, _cudaGetErrorEnum(cuda_error));//24
	fflush(gpFile);

	glEnable(GL_POINT_SPRITE);
	//gPointSprite_ps = LoadKTXTexture("./MEDIA/images/star.ktx", 0);
	//LoadKTXTexture("./MEDIA/images/star.ktx", gPointSprite_ps);
	//LoadTexture(&gPointSprite_ps, MAKEINTRESOURCE(ID_BITMAP_POINT));
	gPointSprite_ps = LoadTexture("./MEDIA/images/snow.dds");
	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d : TexId : %d \n",iErrorIndex++,gPointSprite_ps);//19
	fflush(gpFile);

	fprintf(gpFile, "PrepareRoomScene0_ps Error Index : %d\n",iErrorIndex++);//20
	fflush(gpFile);

    return status;
}

void RenderRoomScene0_ps(mat4 ProjectionMatrix, GLfloat time)
{
	if(cameraposZ < 4.0f)
		return;

	double dftDuration = 0.0;
	void launchCudaKernel(float4* pos, float3 *posDir, float3 *posDirInitial, float3 *posGravity, float *posLife, float *posFade, unsigned int width, unsigned int height, float velocity, float animTime);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static LARGE_INTEGER Frequency;
	static LARGE_INTEGER startTimer;
	static LARGE_INTEGER stopTimer;
	if (bStartTime == true)
	{
		bStartTime = false;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&startTimer);
		QueryPerformanceCounter(&stopTimer);
	}
	else
	{
		QueryPerformanceCounter(&stopTimer);
		LONGLONG timeDifference = stopTimer.QuadPart - startTimer.QuadPart;

		// To get duration in milliseconds
		dftDuration = (double)timeDifference * 1000.0 / (double)Frequency.QuadPart;
		
		if(dftDuration > 12000)
		{
			if (gGpuvelocity <= 5.0f)
			{
				gGpuvelocity += 0.05f;
			}
		}
	}

	glUseProgram(program_room_scene0_ps);
	//glDepthMask(false);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// declaration of metrices
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 translationMatrix;
	mat4 rotationMatrix;
	mat4 scaleMatrix;
	mat4 modelViewProjectionMatrix;

	// initializing above metrices to identity
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	glPointSize(gfSize);

	// do neccessry transformation, rotation, scaling
	translationMatrix = translate(gTransParticleOriginX, gTransParticleOriginY, gTransParticleOriginZ);

	// do necessary matrix multiplication
	//modelViewMatrix = (translationMatrix * scaleMatrix) * rotationMatrix;
	modelMatrix = ((translationMatrix * scaleMatrix)*rotationMatrix);
	/*viewMatrix = lookat(vec3(cameraposX, cameraposY, cameraposZ),
		vec3(cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ),
		vec3(cameraUpX, cameraUpY, cameraUpZ));*/

	modelViewProjectionMatrix = ProjectionMatrix * modelMatrix * viewMatrix;

	// send necessary metrices to shader in respective uniforms
	glUniformMatrix4fv(modelViewProjectionMatrixUinform_room_scene0_ps, 1, GL_FALSE, modelViewProjectionMatrix);

	// bind with vao // this will avoid repetdly binding with vbo
	//glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPointSprite_ps);
	glUniform1i(textureSamplerUniform_room_scene0_ps, 0);

	glUniform1f(u_time_uniform, gfAnimationTime);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_room_scene0_pos_gpu_ps);

	cuda_error = cudaGraphicsMapResources(1, &gpGraphicsResource);
	if (cuda_error != cudaSuccess)
	{
		return ;
	}

	size_t byteCount = 0;
	cuda_error = cudaGraphicsResourceGetMappedPointer((void**)&gpGpuPosition, &byteCount, gpGraphicsResource);
	if (cuda_error != cudaSuccess)
	{
		return;
	}

	/*if (bIsCopyNeeded == true)
	{
		cudaMemcpy(gpGpuPosDir, position_direction, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
		//cudaMemcpy(pPosition, position, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);
	}*/

	//cudaMemcpy(gpGpuPosition, position, MY_ARRAY_SIZE * 4, cudaMemcpyHostToDevice);

	launchCudaKernel(gpGpuPosition, gpGpuPosDir, gpGpuPosDirInitial, gpGpuPosGravity, gpGpuPosLife, gpGpuPosFade, gMeshWidth, gMeshHeight, gGpuvelocity, gfAnimationTime);

	cuda_error = cudaGraphicsUnmapResources(1, &gpGraphicsResource, 0);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	/*glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);*/
	

	// similary bind with texture if any

	GLuint iShape = GL_POINTS;
	
	if (dftDuration < 20000)
	{
		glDrawArrays(iShape, 0, gMeshWidth * gMeshHeight);
	}

	translationMatrix = translate(-gTransParticleOriginX, gTransParticleOriginY, gTransParticleOriginZ);
	modelMatrix = ((translationMatrix * scaleMatrix)*rotationMatrix);
	/*viewMatrix = lookat(vec3(cameraposX, cameraposY, cameraposZ),
		vec3(cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ),
		vec3(cameraUpX, cameraUpY, cameraUpZ));*/
	modelViewProjectionMatrix = ProjectionMatrix * modelMatrix * viewMatrix;
	glUniformMatrix4fv(modelViewProjectionMatrixUinform_room_scene0_ps, 1, GL_FALSE, modelViewProjectionMatrix);
	if (dftDuration < 15000)
	{
		glDrawArrays(iShape, 0, gMeshWidth * gMeshHeight);
	}
	

	translationMatrix = translate(gTransParticleOriginX_3rd, gTransParticleOriginY, gTransParticleOriginZ);
	modelMatrix = ((translationMatrix * scaleMatrix)*rotationMatrix);
	/*viewMatrix = lookat(vec3(cameraposX, cameraposY, cameraposZ),
		vec3(cameraLooingPosX, cameraLooingPosY, cameraLooingPosZ),
		vec3(cameraUpX, cameraUpY, cameraUpZ));*/
	modelViewProjectionMatrix = ProjectionMatrix * ((translationMatrix * scaleMatrix)*rotationMatrix); //modelViewMatrix = (translationMatrix * scaleMatrix) * rotationMatrix;
	glUniformMatrix4fv(modelViewProjectionMatrixUinform_room_scene0_ps, 1, GL_FALSE, modelViewProjectionMatrix);
	if (dftDuration > 10000 /*&& dftDuration < 15000*/)
	{
		gTransParticleOriginX_3rd += 0.001f;
		//glDrawArrays(iShape, 0, (gMeshWidth * gMeshHeight));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind vao
	//glBindVertexArray(0);

	/*if (cameraposZ > -30.0f)
	{
		// if we change this value then we need to change the gTransParticleOriginX value and number of particles
		cameraposZ -= 0.001f;
	}*/
	glDisable(GL_BLEND);
	// un-use the program
	glUseProgram(0);

	gfAnimationTime += 0.009f;
}

void DeallocateRoomScene0_ps(void)
{
	cudaFree(gpGpuPosition);
	cudaFree(gpGpuPosDir);
	cudaFree(gpGpuPosDirInitial);
	cudaFree(gpGpuPosFade);
	cudaFree(gpGpuPosGravity);
	cudaFree(gpGpuPosLife);
	
	if (gpGraphicsResource)
	{
		cudaGraphicsUnregisterResource(gpGraphicsResource);
		gpGraphicsResource = NULL;
	}

	//fprintf(gpFile, "room_scene0.h");
	//printf("room_scene0.h");
	//return;
    if (vbo_room_scene0_pos_ps)
	{
		glDeleteBuffers(1, &vbo_room_scene0_pos_ps);
		vbo_room_scene0_pos_ps = 0;
	}

	if (vbo_room_scene0_pos_gpu_ps)
	{
		glDeleteBuffers(1, &vbo_room_scene0_pos_gpu_ps);
		vbo_room_scene0_pos_gpu_ps = 0;
	}

	fprintf(gpFile, "room_scene0-1.h");

	if (vao_room_scene0_ps)
	{
		glDeleteVertexArrays(1, &vao_room_scene0_ps);
		vao_room_scene0_ps = 0;
	}
	fprintf(gpFile, "room_scene0-2.h");

    // safe Release //
	if (program_room_scene0_ps)
	{
		GLsizei ShaderCount;
		GLsizei ShaderNumber;

		glUseProgram(program_room_scene0_ps);

		// ask program_room_scene0 how many shaders are attached to you
		glGetProgramiv(program_room_scene0_ps, GL_ATTACHED_SHADERS, &ShaderCount);
		GLuint *pShader = (GLuint *)malloc(sizeof(GLuint) * ShaderCount);
		if (pShader)
		{
			// give me attached shaders
			glGetAttachedShaders(program_room_scene0_ps, ShaderCount, &ShaderCount, pShader);
			for (ShaderNumber = 0; ShaderNumber <= ShaderCount; ShaderNumber++)
			{
				// DetachShader //
				glDetachShader(program_room_scene0_ps, pShader[ShaderNumber]);
				glDeleteShader(pShader[ShaderNumber]);
				pShader[ShaderNumber] = 0;
			}// end for loop

			free(pShader);
		} // end of if block of pShader

		glDeleteProgram(program_room_scene0_ps);
		program_room_scene0_ps = 0;
		glUseProgram(0);
	} // end if block of program_room_scene0//*/
}
