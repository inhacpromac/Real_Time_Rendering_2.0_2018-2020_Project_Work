
typedef struct
{
    float x;
    float y;
    float z;
} coordinate;

typedef struct
{
    float u;
    float v;
} tex;

coordinate *vertices    =   NULL;
coordinate *normals     =   NULL;
tex        *texcoords   =   NULL;
int        *indices     =   NULL;

GLuint vao;
GLuint vbo_position;
GLuint vbo_texture;
GLuint vbo_normal;
GLuint vbo_element;

unsigned int vertexCount;
unsigned int indicesCount;

int counter = 0;
void generateMesh(float width, float depth, unsigned int m, unsigned  n)
{   
    void meshPrepareToDraw(void);
	void allocateMeshMemory(int, int);

    // code
    vertexCount  = m*n;
    indicesCount = (m-1)*(n-1)*6;
    //unsigned int faceCount   = (m-1)*(n-1)*2;

    // create the vertices
    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;
    fprintf(gpFile, "half width : %f\n", halfWidth);
    fprintf(gpFile, "half depth : %f\n", halfDepth);

    float dx = width/(n-1);
    float dz = depth/(m-1);
    fprintf(gpFile, "dx : %f\n", dx);
    fprintf(gpFile, "dz : %f\n", dz);

    float du = 10.0f/(n-1);
    float dv = 10.0f/(m-1);

    int i;
    int j;
    int k;

    allocateMeshMemory(vertexCount, indicesCount);

    // calculate vertices, normals and texcoords
    for (i = 0; i < m; ++i)
    {
        float z = halfDepth - i*dz;
        for (j = 0; j < n; ++j)
        {
            float x = -halfWidth + j *dx;
            fprintf(gpFile, "X : %f\n", x);
            vertices[i*n+j].x   = x;
            vertices[i*n+j].y   = 0.0f;
            vertices[i*n+j].z   = z;

            normals[i*n+j].x    = 0.0f;
            normals[i*n+j].y    = 1.0f;
            normals[i*n+j].z    = 0.0f;

            texcoords[i*n+j].u  = j*du;
            texcoords[i*n+j].v  = i*dv;
        }
    }

    // calculate indices
    k = 0;
    for (i = 0; i < m-1; i++)
    {
        for (j = 0; j < n-1; j++)
        {
            indices[k]   = i*n+j;
            indices[k+1] = i*n+j+1;
            indices[k+2] = (i+1)*n+j;
            indices[k+3] = ((i+1)*n+j);
            indices[k+4] = i*n+j+1;
            indices[k+5] = (i+1)*n+j+1;
            k += 6;
        }
    }
    meshPrepareToDraw();
}

void allocateMeshMemory(int vertexCount, int indicesCount)
{
    void cleanupMeshData(void);

    // code
    cleanupMeshData();
    // Error checking remaining
    vertices  = (coordinate *)malloc(vertexCount * sizeof(coordinate));
    normals   = (coordinate *)malloc(vertexCount * sizeof(coordinate));
    texcoords = (tex *)malloc(vertexCount  * sizeof(tex));
    indices   = (int *)malloc(indicesCount * sizeof(int));

    if (!(vertices) || !(normals) || !(texcoords) || !(indices))
	{
		free(vertices);
		free(normals);
        free(texcoords);
        free(indices);
        return;
		//fprintf(gpFile, "Failed to allocate memory in generateCone().\n");
	}
}

void meshPrepareToDraw(void)
{
    void cleanupMeshData(void);

    // code
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// position
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(coordinate), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// normal
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(coordinate), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMALS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// texture
	glGenBuffers(1, &vbo_texture);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texture);
	glBufferData(GL_ARRAY_BUFFER, vertexCount  * sizeof(tex), texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// element 
	glGenBuffers(1, &vbo_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

    // after sending data to GPU, now we can free our arrays
    cleanupMeshData();
}

void cleanupMeshData()
{
    if (indices != NULL)
    {
        free(indices);
        indices = NULL;
    }

    if (vertices != NULL)
    {
        free(vertices);
        vertices = NULL;
    }

    if (normals != NULL)
    {
        free(normals);
        normals = NULL;
    }

    if (texcoords != NULL)
    {
        free(texcoords);
        texcoords = NULL;
    }
}
