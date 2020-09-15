/* 
 * Ref. Superbible 7th Edition - Sourcecode - sb7textoverlay.cpp
 */
    
//Required header files used from main program.
//Hence no header files here.
int     buffer_width;
int     buffer_height;
GLuint  text_buffer;
GLuint  font_texture;
char    *screen_buffer;
GLuint  vao_text_overlay;
GLuint 	vbo;
GLuint  gVertexShaderObject;
GLuint  gFragmentShaderObject;
GLuint  gShaderProgramObject;
bool    dirty;

void text_overlay_init(int width, int height, const char* font)
{
    void text_overlay_teardown(void);

    buffer_width  = width;
    buffer_height = height;

    // *** VERTEX SHADER ***
	// create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCode =
        "#version 440 core\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vec4(float((gl_VertexID >> 1) & 1) * 2.0 - 1.0,\n"
        "                       float((gl_VertexID & 1)) * 2.0 - 1.0,\n"
        "                       0.0, 1.0);\n"
        "}\n";
	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gVertexShaderObject);

    // *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCode =
        "#version 440 core\n"
        "layout (origin_upper_left) in vec4 gl_FragCoord;\n"
        "layout (location = 0) out vec4 o_color;\n"
        "layout (binding = 0) uniform isampler2D text_buffer;\n"
        "layout (binding = 1) uniform isampler2DArray font_texture;\n"
        "void main(void)\n"
        "{\n"
        "    ivec2 frag_coord = ivec2(gl_FragCoord.xy);\n"
        "    ivec2 char_size = textureSize(font_texture, 0).xy;\n"
        "    ivec2 char_location = frag_coord / char_size;\n"
        "    ivec2 texel_coord = frag_coord % char_size;\n"
        "    int character = texelFetch(text_buffer, char_location, 0).x;\n"
        "    float val = texelFetch(font_texture, ivec3(texel_coord, character), 0).x;\n"
        "    if (val == 0.0)\n"
        "        discard;\n"
        "    o_color = vec4(1.0);\n"
        "}\n";
	glShaderSource(gFragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObject);

    
	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	// link shader
	glLinkProgram(gShaderProgramObject);
    
    // vao_text_overlay
    glGenVertexArrays(1, &vao_text_overlay);
    glBindVertexArray(vao_text_overlay);

	float quad[] = 
	{
		1.0, 1.0, 0.0,
		-1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3, quad, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	
    glGenTextures(1, &text_buffer);
    glBindTexture(GL_TEXTURE_2D, text_buffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);

    if (font == NULL)
        if ((font_texture = LoadKTXTexture("./MEDIA/images/cp437_9x16.ktx", 0)) == NULL)
        {
		    fprintf(gpFile,"Error in Loading Font Texture.\n Exitting Now.");
		    text_overlay_teardown();
		    exit(0);
        }

    screen_buffer = (char *)malloc(width * height); // check malloc
    memset(screen_buffer, 0, width * height);       // error check
}


void text_overlay_draw(void)
{
	// start using OpenGL program object
    glUseProgram(gShaderProgramObject);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text_buffer);
    if (dirty)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer_width, buffer_height, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);
        dirty = false;
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font_texture);

    glBindVertexArray(vao_text_overlay);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void text_overlay_drawText(const char* str, int x, int y)
{ 
    char* dst = screen_buffer + y * buffer_width + x;
    strcpy(dst, str);
    dirty = true;
}

void text_overlay_clear()
{
    memset(screen_buffer, 0, buffer_width * buffer_height);
    dirty = true;
}

void text_overlay_teardown()
{
	glDetachShader(gShaderProgramObject, gVertexShaderObject);
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;

	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;

    if(text_buffer)
    {
        glDeleteTextures(1, &text_buffer);
        text_buffer = 0;
    }
    
    if (font_texture)
    {
        glDeleteTextures(1, &font_texture);
        font_texture = 0;
    }

    if (vao_text_overlay)
    {
        glDeleteVertexArrays(1, &vao_text_overlay);
        vao_text_overlay = 0;
    }

    if (screen_buffer)
    {
        free(screen_buffer);
        screen_buffer = 0;
    }
}
