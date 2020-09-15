GLuint vao_room_scene0;
GLuint vbo_room_scene0;
GLuint program_room_scene0;

GLuint modelMatrixUinform_room_scene0;
GLuint viewMatrixUniform_room_scene0;
GLuint projectionMatrixUniform_room_scene0;

GLuint mixFactorUniform_room_scene0;
GLfloat mixFactor_room_scene0;
GLuint mixColorBUniform_room_scene0;
vec4   mixColorB_room_scene0;

GLuint ambientLightUniform_room_scene0;
GLuint diffuseLightUniform_room_scene0;
GLuint specularLightUniform_room_scene0;
GLuint ambientMaterialUniform_room_scene0;
GLuint diffuseMaterialUniform_room_scene0;
GLuint specularMaterialUniform_room_scene0;
GLuint shininessUniform_room_scene0;
GLuint lightPositionUniform_room_scene0;

GLuint whitePixelTexture_room_scene0;
GLuint textureSamplerUniform_room_scene0;


Light lights_room_scene0[MAX_LIGHTS_SCENE0];
Model models_room_scene0[MAX_OBJECTS_SCENE0];

GLfloat rAngle_room_scene0;

GLint PrepareRoomScene0(void)
{
	GLint status = 0;

	// *** SHADER BLOCK ***
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;


	// *** VERTEX SHADER ***
	GLuint vs = LoadShaders("./MEDIA/shaders/vertexshader_room_scene0.vs", GL_VERTEX_SHADER);
	if (!vs)
    return -1;

	// *** FRAGMENT SHADER ***
	GLuint fs = LoadShaders("./MEDIA/shaders/fragmentshader_room_scene0.fs", GL_FRAGMENT_SHADER);
	if (!fs)
	return -1;

	// *** SHADER PROGRAM ***
	// create 
	program_room_scene0 = glCreateProgram();

	glAttachShader(program_room_scene0, vs);
	glAttachShader(program_room_scene0, fs);

	// prelinking binding to vertex attribute //
	glBindAttribLocation(program_room_scene0, AMC_ATTRIBUTE_POSITION,  "vPosition");
	glBindAttribLocation(program_room_scene0, AMC_ATTRIBUTE_NORMALS,   "vNormal");
	glBindAttribLocation(program_room_scene0, AMC_ATTRIBUTE_TEXCOORD0, "vTexcoord");


	// Link the shader program_room_scene0
	glLinkProgram(program_room_scene0);
	glGetProgramiv(program_room_scene0, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(program_room_scene0, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(program_room_scene0, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
                return -1;
			}
		}
	}

	glDetachShader(program_room_scene0, vs);
	glDeleteShader(vs);
	//vs = 0;
	glDetachShader(program_room_scene0, fs);
	glDeleteShader(fs);
	//fs = 0;

	// postlinking receiving uniform location //
	modelMatrixUinform_room_scene0 			= glGetUniformLocation(program_room_scene0, "modelMatrix");
	viewMatrixUniform_room_scene0  			= glGetUniformLocation(program_room_scene0, "viewMatrix");
	projectionMatrixUniform_room_scene0 	= glGetUniformLocation(program_room_scene0, "projectionMatrix");
	
	mixFactorUniform_room_scene0			= glGetUniformLocation(program_room_scene0, "mixFactor");
	mixColorBUniform_room_scene0			= glGetUniformLocation(program_room_scene0, "colorB");
	
	ambientLightUniform_room_scene0  		= glGetUniformLocation(program_room_scene0, "ambientLight");
	diffuseLightUniform_room_scene0  		= glGetUniformLocation(program_room_scene0, "diffuseLight");
	specularLightUniform_room_scene0 		= glGetUniformLocation(program_room_scene0, "specularLight");
	lightPositionUniform_room_scene0 		= glGetUniformLocation(program_room_scene0, "lightPosition");

	ambientMaterialUniform_room_scene0  	= glGetUniformLocation(program_room_scene0, "ambientMaterial");
	diffuseMaterialUniform_room_scene0  	= glGetUniformLocation(program_room_scene0, "diffuseMaterial");
	specularMaterialUniform_room_scene0 	= glGetUniformLocation(program_room_scene0, "specularMaterial");
	shininessUniform_room_scene0  			= glGetUniformLocation(program_room_scene0, "shininess");

	textureSamplerUniform_room_scene0 		= glGetUniformLocation(program_room_scene0, "textureSampler");


	// *** OBJECT LOADING ***

	// debug
	fprintf(gpFile, "\n");

	// Load Obj Model And Prepare To Draw
	if (LoadObjModels(MAX_OBJECTS_SCENE0, models_room_scene0, paths) < 0)
	{
		fprintf(gpFile, "obj Models or Related Files Did Not Get Loaded Successfully.Exitting Now...\n");
		return -1;
	}
	else
	{
		// debug
		fprintf(gpFile, "\n");
		fprintf(gpFile, "obj Models Loaded Successfully.\n");
		fprintf(gpFile, "Total Triangle Count - %d\n", TotalTriangles);
	}

	// create default texture
	whitePixelTexture_room_scene0 = createWhitePixelTexture();

	mixFactor_room_scene0 = 0.0f;

	// light and mterial properties
	// default light
	lights_room_scene0[0].lightpos      		= vec3(1.0f, 1.0f, 1.0f);
	lights_room_scene0[0].ambientlight  		= vec4(0.50f, 0.20f, 0.0f, 1.0f);
	lights_room_scene0[0].diffuselight  		= vec4(0.80f, 0.30f, 0.0f, 1.0f);
	lights_room_scene0[0].specularlight 		= vec4(1.0f,  1.0f, 1.0f, 1.0f);
	// default material
	lights_room_scene0[0].shininess      		= 1.0f;
	lights_room_scene0[0].ambientmaterial  	= vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lights_room_scene0[0].diffusematerial  	= vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lights_room_scene0[0].specularmaterial 	= vec4(1.0f, 1.0f, 1.0f, 1.0f);

	mixColorB_room_scene0 = vec4(1.0, 1.0, 1.0, 1.0);

    return status;
}


void RenderRoomModelScene0(int index)
{
	glBindVertexArray(models_room_scene0[index].vao_model);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models_room_scene0[index].vbo_model_element);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned int g = 0; g < models_room_scene0[index].pGroups.size(); g++)
    {
        Group currentGroup = models_room_scene0[index].pGroups.at(g);
        Material currentMaterial = models_room_scene0[index].pMaterials.at(0); // default material

        // if material is defined
        if (models_room_scene0[index].pMaterials.size() > 1)
        currentMaterial = models_room_scene0[index].pMaterials.at(currentGroup.materialIndex);
                                                                  
		// send material properties
		glUniform3fv(ambientMaterialUniform_room_scene0,  1, currentMaterial.ambient);
		glUniform3fv(diffuseMaterialUniform_room_scene0,  1, currentMaterial.diffuse);
		glUniform3fv(specularMaterialUniform_room_scene0, 1, currentMaterial.specular);

		// texture
		glActiveTexture(GL_TEXTURE0);
        if(currentMaterial.has_map_Kd) 
		glBindTexture(GL_TEXTURE_2D, models_room_scene0[index].pTextures.at(currentMaterial.index_map_Kd).id);
        else 
		glBindTexture(GL_TEXTURE_2D, whitePixelTexture_room_scene0);
		glUniform1i(textureSamplerUniform_room_scene0, 0);

		// draw
		glDrawElements(GL_TRIANGLES, currentGroup.triangles.size() * 3, GL_UNSIGNED_INT, (void *)((currentGroup.stride) * 3 * sizeof(GLuint)));
        
		glBindTexture(GL_TEXTURE_2D, 0);
    }
	glBindVertexArray(0);
}

void RenderRoomScene0(mat4 ProjectionMatrix, GLfloat time)
{
	
	// Start Using shader program_room_scene0 object //
	glUseProgram(program_room_scene0);

	// declaration of matrices //
	mat4 modelMatrix;
	mat4 translationMatrix;
	mat4 rotationMatrix;

	// initialization above matrices identity //
	modelMatrix 				= mat4::identity();
	viewMatrix 					= mat4::identity();
	translationMatrix 			= mat4::identity();

	translationMatrix 			= translate(0.0f, -1.0f, -2.0f);
	modelMatrix 				= translationMatrix;
	rotationMatrix				= rotate(rAngle_room_scene0, 0.0f, 1.0f, 0.0f);
	modelMatrix					= modelMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(modelMatrixUinform_room_scene0, 			1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_room_scene0,  			1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_room_scene0, 		1, GL_FALSE, ProjectionMatrix);

	glUniform3fv(ambientLightUniform_room_scene0,  	1, lights_room_scene0[0].ambientlight);
	glUniform3fv(diffuseLightUniform_room_scene0,  	1, lights_room_scene0[0].diffuselight);
	glUniform3fv(specularLightUniform_room_scene0, 	1, lights_room_scene0[0].specularlight);
	glUniform4fv(lightPositionUniform_room_scene0, 	1, lights_room_scene0[0].lightpos);
	glUniform1f(shininessUniform_room_scene0,		lights_room_scene0[0].shininess);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(mixFactorUniform_room_scene0, mixFactor_room_scene0);

	glUniform4fv(mixColorBUniform_room_scene0, 1, mixColorB_room_scene0);
	
	// Render obj model
	RenderRoomModelScene0(0);

	//glDisable(GL_BLEND);
	
	glUseProgram(0);

	if (time > 6.0f)
		if (mixFactor_room_scene0 < 1.0f)
			mixFactor_room_scene0 += 0.005f;

	rAngle_room_scene0 += 0.5f;
	if (rAngle_room_scene0 >= 360.0f)
		rAngle_room_scene0 = 0.0f;
}

void DeallocateRoomScene0(void)
{
	fprintf(gpFile, "room_scene.h");
	return;
    if (vbo_room_scene0)
	{
		glDeleteBuffers(1, &vbo_room_scene0);
		vbo_room_scene0 = 0;
	}

	if (vao_room_scene0)
	{
		glDeleteVertexArrays(1, &vao_room_scene0);
		vao_room_scene0 = 0;
	}

	// delete obj models_room_scene0
	for (int i = 0; i < MAX_OBJECTS_SCENE0; i++)
	{
		if (models_room_scene0[i].vao_model)
		{
			glDeleteVertexArrays(1, &models_room_scene0[i].vao_model);
			models_room_scene0[i].vao_model = 0;
		}

		if (models_room_scene0[i].vbo_model_position)
		{
			glDeleteBuffers(1, &models_room_scene0[i].vbo_model_position);
			models_room_scene0[i].vbo_model_position = 0;
		}

		if (models_room_scene0[i].vbo_model_normal)
		{
			glDeleteBuffers(1, &models_room_scene0[i].vbo_model_normal);
			models_room_scene0[i].vbo_model_normal = 0;
		}

		if (models_room_scene0[i].vbo_model_texture)
		{
			glDeleteBuffers(1, &models_room_scene0[i].vbo_model_texture);
			models_room_scene0[i].vbo_model_texture = 0;
		}

		if (models_room_scene0[i].vbo_model_element);
		{
			glDeleteBuffers(1, &models_room_scene0[i].vbo_model_element);
			models_room_scene0[i].vbo_model_element = 0;
		}

		//delete textures
		for(unsigned int i = 0; i < models_room_scene0[i].pTextures.size(); i++)
		{
			if(glIsTexture(models_room_scene0[i].pTextures.at(i).id))
			glDeleteTextures(1, &models_room_scene0[i].pTextures.at(i).id);
			models_room_scene0[i].pTextures.at(i).id = 0;
		}
		models_room_scene0[i].pTextures.clear();
		models_room_scene0[i].pGroups.clear();
		models_room_scene0[i].pMaterials.clear();
	}

    // safe Release //
	if (program_room_scene0)
	{
		GLsizei ShaderCount;
		GLsizei ShaderNumber;

		glUseProgram(program_room_scene0);

		// ask program_room_scene0 how many shaders are attached to you
		glGetProgramiv(program_room_scene0, GL_ATTACHED_SHADERS, &ShaderCount);
		GLuint *pShader = (GLuint *)malloc(sizeof(GLuint) * ShaderCount);
		if (pShader)
		{
			// give me attached shaders
			glGetAttachedShaders(program_room_scene0, ShaderCount, &ShaderCount, pShader);
			for (ShaderNumber = 0; ShaderNumber <= ShaderCount; ShaderNumber++)
			{
				// DetachShader //
				glDetachShader(program_room_scene0, pShader[ShaderNumber]);
				glDeleteShader(pShader[ShaderNumber]);
				pShader[ShaderNumber] = 0;
			}// end for loop

			free(pShader);
		} // end of if block of pShader

		glDeleteProgram(program_room_scene0);
		program_room_scene0 = 0;
		glUseProgram(0);
	} // end if block of program_room_scene0//
}