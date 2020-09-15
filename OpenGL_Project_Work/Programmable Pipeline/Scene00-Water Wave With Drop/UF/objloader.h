/*
* Ref. Book - GPU Pro 3 - 
* Chapter - Near Field Global Illuimation
*/

#include "objmodelstructs.h"

std::vector<Group>          mGroups;
std::vector<Material>       mMaterials;
std::vector<Vertex>         mVertices;
std::vector<Vertex>         mNormals;
std::vector<TexCoord>       mTexcoords;
std::vector<Triangle>       mTriangles;
std::vector<Texture>        mTextures;

std::string mMtlLibName;
std::string mPathModel;

// sorted overhead
std::vector<GLfloat>       sVertices;
std::vector<GLfloat>       sNormals;
std::vector<GLfloat>       sTexcoords;
std::vector<GLuint>        sIndices;

int TotalTriangles;

Vertex convertToVector(const std::string& token1, const std::string& token2, const std::string& token3)
{
   return Vertex(   
                    static_cast<float>(atof(token1.c_str())),
                    static_cast<float>(atof(token2.c_str())),
                    static_cast<float>(atof(token3.c_str()))
                );
}


TexCoord convertToTexCoord(const std::string& token1, const std::string& token2)
{
   return TexCoord(
                    static_cast<float>(atof(token1.c_str())),
                    static_cast<float>(atof(token2.c_str()))
                  );
}

unsigned int AddGroup(const std::string& name)
{
    Group group;
    group.name = name;
    group.materialIndex = 0; // the default material
    mGroups.push_back(group);

    return mGroups.size()-1;
}

unsigned int FindOrAddMaterial(const std::string& name)
{
    // debug
    fprintf(gpFile, "\t\tEenteing - FindOrAddMaterial(const std::string& name)\n");

    for (unsigned int i = 0; i < mMaterials.size(); i++)
    {
        if (!name.compare(mMaterials.at(i).name))
        {
            fprintf(gpFile, "\t\tExitting - FindOrAddMaterial(const std::string& name)\n");
            return i;
        }
    }

    // did not find material
    fprintf(gpFile, "\t\t\tFindOrAddMaterial(): can't find material %s\n", name.c_str());

    fprintf(gpFile, "\t\tExitting - FindOrAddMaterial(const std::string& name)\n");

    return 0;
}


std::string DirName(std::string path)
{
   size_t found;
   
   found = path.find_last_of("/\\");
   std::string folder = (found!=std::string::npos)?path.substr(0, found) : "";
   
   return folder;
}


unsigned int FindOrAddTexture(std::string file)
{
    fprintf(gpFile, "\t\tEentering - FindOrAddTexture(std::string file)\n");

    // check whether a texture with this filename has already been loaded
    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        if (!file.compare(mTextures.at(i).filename))
        {
            // found texture !
            fprintf(gpFile, "\t\tExitting - FindOrAddTexture(std::string file)\n");
            return i;
        }
    }

    // did not find texture
    // load it

    Texture tex;
    int width, height;
    
    tex.filename    = file;
    tex.id          = LoadTexture(file.c_str());
    //tex.width     = width;
    //tex.height    = height;

    mTextures.push_back(tex);

    fprintf(gpFile, "\t\tExitting - FindOrAddTexture(std::string file)\n");

    return (mTextures.size()-1);
}

void ReadMTL(std::string file)
{   
   // code

   // debug
   fprintf(gpFile, "\t\tEntering - ReadMTL(std::string file)\n");

   std::string dirObj = DirName(mPathModel);
   std::string dirMtl = DirName(file);
   std::string mtlfile = dirObj+"/"+file;

    std::ifstream inFile(mtlfile.c_str());
    
    if (!inFile)
    {
        fprintf(gpFile, "\t\t.mtl File Could Not Be Loaded.\n");
        return;
    }

    Material defaultMaterial = mMaterials.at(0);
    
    // now read in the data
    std::string line;
    while (getline(inFile, line))
    {
        std::stringstream lineStream(line);
        std::string token;
        std::vector<std::string> tokens;

        // get all tokens
        while (lineStream >> token)
        {
            tokens.push_back(token);
        }

        // if there are tokens
        if (!tokens.empty())
        {
            // process tokens

            if (!tokens.at(0).compare("newmtl"))
            {
                // tokens size > 1
                mMaterials.push_back(defaultMaterial);
                mMaterials.back().name = tokens.at(1);
                //fprintf(gpFile, "usemtl - %s\n",  tokens.at(1).c_str());
            }
            // ambient reflectivity
            else if (!tokens.at(0).compare("Ka"))
            {
                // tokens size > 3
                mMaterials.back().ambient[0] = GLfloat(atof(tokens.at(1).c_str()));
                mMaterials.back().ambient[1] = GLfloat(atof(tokens.at(2).c_str()));
                mMaterials.back().ambient[2] = GLfloat(atof(tokens.at(3).c_str()));
            }
            // diffuse reflectivity
            else if (!tokens.at(0).compare("Kd"))
            {
                // tokens size > 3
                mMaterials.back().diffuse[0] = GLfloat(atof(tokens.at(1).c_str()));
                mMaterials.back().diffuse[1] = GLfloat(atof(tokens.at(2).c_str()));
                mMaterials.back().diffuse[2] = GLfloat(atof(tokens.at(3).c_str()));
            }
            // specular reflectivity
            else if (!tokens.at(0).compare("Ks"))
            {
                // tokens size > 3
                mMaterials.back().specular[0] = GLfloat(atof(tokens.at(1).c_str()));
                mMaterials.back().specular[1] = GLfloat(atof(tokens.at(2).c_str()));
                mMaterials.back().specular[2] = GLfloat(atof(tokens.at(3).c_str()));
            }
            // diffuse texture map
            else if (!tokens.at(0).compare("map_Kd"))
            {
                // size > 1
                mMaterials.back().index_map_Kd = FindOrAddTexture(tokens.at(1));
                mMaterials.back().has_map_Kd   = true;
            }
        }
    }

    // debug
    fprintf(gpFile, "\t\tExitting - ReadMTL(std::string file)\n");
}

bool LoadObj(std::string file, int index, Model models[])
{
    void printLog(void);
    void sortArrays(void);
    void prepareToRender(int, Model models[]);

    // code

    // debug 
    fprintf(gpFile, "\tEntering - LoadObj(std::string file, int index)\n");
    fprintf(gpFile, "\tFile %s\n", file.c_str());

    // obj log
    fprintf(objlog, "Model Index %d\n", index);

    // open file
    std::ifstream inFile(file.c_str());
    if (!inFile)
    {
        fprintf(gpFile, "\tCould Not Open .obj File.\n");
        return false;
    }

    // model data
    mPathModel = file;

    Vertex      dummyVector;
    TexCoord    dummyTexcoord;
    Triangle    dummyTriangle;

    mVertices.push_back(dummyVector);
    mNormals.push_back(dummyVector);
    mTexcoords.push_back(dummyTexcoord);
    mTriangles.push_back(dummyTriangle);

    // make a default group
    // obj spec: The default group name is default
    unsigned int currentGroupIndex = AddGroup("default");
    
    // group size > 0
    unsigned int currentMaterialIndex = 0; // default material

    // set default material
    Material defaultMaterial;
    defaultMaterial.name = "";
    defaultMaterial.id   = 0;
    defaultMaterial.diffuse[0]  = 0.8f;
    defaultMaterial.diffuse[1]  = 0.8f;
    defaultMaterial.diffuse[2]  = 0.8f;
    defaultMaterial.diffuse[3]  = 1.0f;
    defaultMaterial.ambient[0]  = 0.2f;
    defaultMaterial.ambient[1]  = 0.2f;
    defaultMaterial.ambient[2]  = 0.2f;
    defaultMaterial.ambient[3]  = 1.0f;
    defaultMaterial.specular[0] = 0.0f;
    defaultMaterial.specular[1] = 0.0f;
    defaultMaterial.specular[2] = 0.0f;
    defaultMaterial.specular[3] = 1.0f;

    defaultMaterial.has_map_Kd      = false;
    defaultMaterial.index_map_Kd    = 0;

    mMaterials.push_back(defaultMaterial);


    // first find "mtllib" in file and read it
    std::string line;
    std::stringstream lineStream;
    std::string token;
    std::vector<std::string> tokens;

    // read in file
    while (getline(inFile, line))
    {
        // a line was successfully read
        // tokenize the line using a string stream

        lineStream.clear();
        lineStream.str(line);
        tokens.clear();

        // get all tokens
        while (lineStream >> token)
        {
            tokens.push_back(token);
        }

        // if there are tokens
        if (!tokens.empty())
        {
            // process tokens

            // mtlib : material library name
            if(!tokens.at(0).compare("mtllib"))
            {
                // tokens size > 1
                mMtlLibName = tokens.at(1);
                
                // debug
                fprintf(gpFile, "\tmtllib detected : %s\n", mMtlLibName.c_str());

                // data log
                fprintf(objlog, "mtllib detected : %s\n", mMtlLibName.c_str());

                ReadMTL(mMtlLibName);

                break;
            }
        }
    }

    // read in fie
    inFile.clear();                 // forget we hit the end of file
    inFile.seekg(0, std::ios::beg); // move to the start of the file
    std::stringstream strStream;

    while (getline(inFile, line))
    {
        // tokenize line
        lineStream.clear();
        lineStream.str(line);
        tokens.clear();

        while(lineStream >> token)
        {
            tokens.push_back(token);
        }

        // if there are tokens
        if (!tokens.empty())
        {
            // process tokens

            // v: vertex
            if (!tokens.at(0).compare("v"))
            {
                // size >= 4
                mVertices.push_back(convertToVector(tokens.at(1), tokens.at(2), tokens.at(3)));
                
                // debug
                //fprintf(gpFile, "%s\t%s\t%s\t\n", tokens.at(1).c_str(), tokens.at(2).c_str(), tokens.at(3).c_str());
            }
            // vt: vertex texture coordinate
            else if (!tokens.at(0).compare("vt"))
            {
                // tokens size >= 3
                mTexcoords.push_back(convertToTexCoord(tokens.at(1), tokens.at(2)));
                
                // debug
                //fprintf(gpFile, "%s\t%s\t\n", tokens.at(1).c_str(), tokens.at(2).c_str());
            }
            // vn:: vertex normal
            else if (!tokens.at(0).compare("vn"))
            {
                // size >= 4
                mNormals.push_back(convertToVector(tokens.at(1), tokens.at(2), tokens.at(3)));
                
                // debug
                //fprintf(gpFile, "%s\t%s\t%s\t\n", tokens.at(1).c_str(), tokens.at(2).c_str(), tokens.at(3).c_str());
            }
            // f: face
            else if (!tokens.at(0).compare("f"))
            {
                // for triangle there must be 3 further tokens
                // size > 3

                // create triangles from plygons with more than 3 vertices:
                // number of triangles = num(tokens after "f") - 2;
                int numTrinagles = tokens.size()-1-2;
                
                // debug
                //fprintf(gpFile, " num triangles %d\n", numTrinagles);
                for (int t = 0; t < numTrinagles; t++)
                {
                    // make an empty triangle
                    Triangle triangle;
                    
                    int tokenIndcies[] = {1, 2+t, 3+t};

                    for (int vertex=1; vertex<=3; vertex++)
                    {
                        triangle.vIndices[vertex-1]  = 0;
                        triangle.vtIndices[vertex-1] = 0;
                        triangle.vnIndices[vertex-1] = 0;

                        std::string vertexToken;
                        strStream.clear();
                        strStream.str(tokens.at(tokenIndcies[vertex-1]));

                        int count = 0;
                        while (getline(strStream, vertexToken, '/'))
                        {
                            if (!vertexToken.empty())
                            {
                                switch(count)
                                {
                                    case 0:
                                        triangle.vIndices[vertex-1] = atoi(vertexToken.c_str());
                                        break;
                                    case 1:
                                        triangle.vtIndices[vertex-1] = atoi(vertexToken.c_str());
                                        break;
                                    case 2:
                                        triangle.vnIndices[vertex-1] = atoi(vertexToken.c_str());
                                        break;
                                    default:
                                        break;
                                }
                            }
                            count++;
                        }

                        // debug
                        //fprintf(gpFile, "v  : %d\n", triangle.vIndices[vertex-1]);
                        //fprintf(gpFile, "vt : %d\n", triangle.vtIndices[vertex-1]);
                        //fprintf(gpFile, "vn : %d\n", triangle.vnIndices[vertex-1]);
                    }
                    // done with triangle

                    // add triangle to mTriangles vector
                    mTriangles.push_back(triangle);

                    // debug
                    //fprintf(gpFile, "%d\n", mTriangles.size());

                    unsigned int triangleIndex = mTriangles.size()-1;
                    // debug
                    //fprintf(gpFile, "%d\n", triangleIndex);

                    // add index of current triangle to current group
                    mGroups.at(currentGroupIndex).triangles.push_back(triangleIndex);
                }
            }
            // o: object group
            else if (!tokens.at(0).compare("o"))
            {
                // size > 1
                currentGroupIndex = AddGroup(tokens.at(1));
                
                mGroups.at(currentGroupIndex).materialIndex = currentMaterialIndex;
                mGroups.at(currentGroupIndex).stride = mTriangles.size()-1;

                // debug
                //fprintf(gpFile, "current group : %d\n", currentGroupIndex);
                //fprintf(gpFile, "current material : %d\n", currentMaterialIndex);
                //fprintf(gpFile, "groups : %d\n", mGroups.size());
                //fprintf(gpFile, "stride %d\n",   mGroups.at(currentGroupIndex).stride);
            }
            // usemtl : material name
            else if (!tokens.at(0).compare("usemtl"))
            {
                // debug
                fprintf(gpFile, "\tusemtl : %s\n", tokens.at(1).c_str());

                // size > 1
                currentMaterialIndex = FindOrAddMaterial(tokens.at(1));
                
                mGroups.at(currentGroupIndex).materialIndex = currentMaterialIndex;
               
                // debug
                //fprintf(gpFile, "mat index : %d\n", currentMaterialIndex);
            }

        }// tokens

        // new line
    }

    inFile.close();

    // data log
    fprintf(objlog, "Total Groups       : %d\n",   (int)mGroups.size()-1); // group size minus default group
    fprintf(objlog, "Vertices           : %d\tTotal Vertices    (Vertices  * 3 x y z) : %d\n",  (int)mVertices.size(),  (int)mVertices.size()  * 3);
    fprintf(objlog, "Texcoords          : %d\tTotal Texcoords   (Texcoords * 2 u v)   : %d\n",  (int)mTexcoords.size(), (int)mTexcoords.size() * 2);
    fprintf(objlog, "Normals            : %d\tTotal Normals     (Normals   * 3 x y z) : %d\n",  (int)mNormals.size(),   (int)mNormals.size()   * 3);
    fprintf(objlog, "Total Triangles    : %d\tTotal Indices     (Total Triangles * 3) : %d\n",  (int)mTriangles.size(), (int)mTriangles.size() * 3);

    models[index].pGroups    = mGroups;
    models[index].pMaterials = mMaterials;
    models[index].pTextures  = mTextures;

    
    TotalTriangles += mTriangles.size();

    printLog();
    sortArrays();
    prepareToRender(index, models);

    // debug
    fprintf(gpFile, "\tExitting - LoadObj(std::string file, int index)\n");

    return true;
}


int LoadObjModels(GLuint maxobjects, Model models[], const char *paths[])
{
    // code

    // debug
    fprintf(gpFile, "Entering - LoadObjModels(void)\n");

    // open the data obj log file
    objlog = fopen("objmodellog.txt", "w");
    if (objlog == NULL)
    {
        fprintf(gpFile, "\tCould Create/Open Obj Data Log File.Exitting Now...\n");
        return -1;
    }

    for (int i = 0; i < maxobjects; i++)
    LoadObj(paths[i], i, models);
    
    // debug
    fprintf(gpFile, "Exitting - LoadObjModels(void)\n");

    return 0;
}


GLuint createWhitePixelTexture(void)
{
    GLuint texture;
	GLfloat data[] = {1, 1, 1};
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void prepareToRender(int index, Model models[])
{   
    // code
    
    void cleanUp(void);

    // create Vao //
    glGenVertexArrays(1, &models[index].vao_model);
    glBindVertexArray(models[index].vao_model);
    // position
    glGenBuffers(1, &models[index].vbo_model_position);
    glBindBuffer(GL_ARRAY_BUFFER, models[index].vbo_model_position);
    glBufferData(GL_ARRAY_BUFFER, sVertices.size() * sizeof(GLfloat), (const void*)&sVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // normal
    glGenBuffers(1, &models[index].vbo_model_normal);
    glBindBuffer(GL_ARRAY_BUFFER, models[index].vbo_model_normal);
    glBufferData(GL_ARRAY_BUFFER, sNormals.size() * sizeof(GLfloat), (const void*)&sNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // texture
    glGenBuffers(1, &models[index].vbo_model_texture);
    glBindBuffer(GL_ARRAY_BUFFER, models[index].vbo_model_texture);
    glBufferData(GL_ARRAY_BUFFER, sTexcoords.size() * sizeof(GLfloat), (const void*)&sTexcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // sIndices
    glGenBuffers(1, &models[index].vbo_model_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[index].vbo_model_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sIndices.size() * sizeof(GLuint), (const void *)&sIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    cleanUp();
}


void cleanUp(void)
{

    mGroups.clear();
    mMaterials.clear();
    mVertices.clear();
    mNormals.clear();
    mTexcoords.clear();
    mTriangles.clear();
    mTextures.clear();

    // sorted overhead
    sVertices.clear();
    sNormals.clear();
    sTexcoords.clear();
    sIndices.clear();
}

void RenderObject(int index, Model models[])
{
	glBindVertexArray(models[index].vao_model);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[index].vbo_model_element);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    for(unsigned int g = 0; g < models[index].pGroups.size(); g++)
    {
        Group currentGroup = models[index].pGroups.at(g);
        Material currentMaterial = models[index].pMaterials.at(0); // default material

        // if material is defined
        if (models[index].pMaterials.size() > 1)
        currentMaterial = models[index].pMaterials.at(currentGroup.materialIndex);
        
        //glBindTexture(GL_TEXTURE_2D, 0);

        if(currentMaterial.has_map_Kd)
        glBindTexture(GL_TEXTURE_2D, models[index].pTextures.at(currentMaterial.index_map_Kd).id);
        glDrawElements(GL_TRIANGLES, currentGroup.triangles.size() * 3, GL_UNSIGNED_INT, (void *)((currentGroup.stride) * 3 * sizeof(GLuint)));
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    //glDrawElements(GL_TRIANGLES, sIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void sortArrays(void)
{
   int k = 0;
   // iterate through all groups
   for(unsigned int g = 0; g < mGroups.size(); g++)
   {
        Group currentGroup = mGroups.at(g);

        // iterate through ech triangle of the group
        for (unsigned int t = 0; t < currentGroup.triangles.size(); t++)
        {
            Triangle triangle = mTriangles.at(currentGroup.triangles.at(t));

            // iterate through each coordinates of the triangle
            for (int idx = 0; idx <= 2; idx++)
            {
                // vertices
                sVertices.push_back(mVertices.at(triangle.vIndices[idx])[0]);
                sVertices.push_back(mVertices.at(triangle.vIndices[idx])[1]);
                sVertices.push_back(mVertices.at(triangle.vIndices[idx])[2]);
                // indices
                sIndices.push_back(k++);

                // texcoords
                sTexcoords.push_back(mTexcoords.at(triangle.vtIndices[idx])[0]);
                sTexcoords.push_back(mTexcoords.at(triangle.vtIndices[idx])[1]);
                // normals
                sNormals.push_back(mNormals.at(triangle.vnIndices[idx])[0]);
                sNormals.push_back(mNormals.at(triangle.vnIndices[idx])[1]);
                sNormals.push_back(mNormals.at(triangle.vnIndices[idx])[2]);
            }
        }
   }

    // debug
   //fprintf(gpFile, "sorted vertices size %d\n", sVertices.size());
   //fprintf(gpFile, "sorted normals size %d\n", sNormals.size());
   //fprintf(gpFile, "Indices size %d\n", sIndices.size());
}

void printLog()
{
    // iterate through all groups - skip default group
    fprintf(objlog, "-------------------------------------------\n");
    for(unsigned int g = 1; g < mGroups.size(); g++)
    {
        Group currentGroup = mGroups.at(g);
        
        Material currentMaterial = mMaterials.at(0);
        if (mMaterials.size() > 1)
        currentMaterial = mMaterials.at(currentGroup.materialIndex);

        fprintf(objlog, "\tGroup Index        : %d\n", g);
        fprintf(objlog, "\tGroup Name         : %s\n", currentGroup.name.c_str());
        fprintf(objlog, "\tTriangles          : %d\n", (int)currentGroup.triangles.size());
        fprintf(objlog, "\tMaterial Index     : %d\n", currentGroup.materialIndex);
        fprintf(objlog, "\tStride             : %i\n", currentGroup.stride);
        fprintf(objlog, "\tusemtl             : %s\n", currentMaterial.name.c_str());
        fprintf(objlog, "\tHas map Kd         : %d\n", (currentMaterial.has_map_Kd) ? 0 : 1);
        //fprintf(objlog, "Texture              : %d\n", currentGroup.has_map_Kd ? 0 : 1);
        fprintf(objlog, "\t-----------------------\n");

    }
    fprintf(objlog, "-------------------------------------------\n");
}
