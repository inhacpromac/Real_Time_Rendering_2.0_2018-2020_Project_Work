#pragma once

struct Triangle
{
    unsigned int vIndices[3];
    unsigned int vnIndices[3];
    unsigned int vtIndices[3];
};

typedef vmath::vec3 Vertex;

typedef vmath::vec2 TexCoord;

struct Group
{
    std::string name;
    std::vector<unsigned int> triangles;
    unsigned int stride;
    unsigned int materialIndex;
};

struct Material
{
  std::string name;             /* name of material */
  int id;
  GLfloat diffuse[4];           /* diffuse component */
  GLfloat ambient[4];           /* ambient component */
  GLfloat specular[4];          /* specular component */
  GLfloat shininess;            /* specular exponent */
  bool has_map_Kd;
  unsigned int index_map_Kd;    ///< index pointing to diffuse color map
};

struct Texture
{
    std::string filename;
    GLuint id;
    int width;
    int height;
};

struct Model
{
    std::vector<Group>      pGroups;
    std::vector<Material>   pMaterials;
    std::vector<Texture>    pTextures;
    GLuint                  vao_model;
    GLuint                  vbo_model_position;
    GLuint                  vbo_model_normal;
    GLuint                  vbo_model_texture;
    GLuint                  vbo_model_element;
};

struct ModelPath
{
    char *path;
};



