
#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>



namespace loader{


struct Dummy_Colours
{
   std::vector<glm::vec3> colour_array =
   {
      glm::vec3(1.0,0.0,0.0),
      glm::vec3(1.0,1.0,0),
      glm::vec3(0.0,1.0,0),
      glm::vec3(1.0,0.0,1.0),
      glm::vec3(1.0,1.0,1.0),
      glm::vec3(1.0,1.0,0.4),
      glm::vec3(0.2,1.0,0),
      glm::vec3(0.3,1.0,0.8),
      glm::vec3(0.2,0.0,0.3),
      glm::vec3(1.0,1.0,0.3),
      glm::vec3(0.1,0.1,0.9),
      glm::vec3(0.5,0.3,0),
   };
};


struct Texture
{
    std::string Path_to_Texture;
};


struct OpenGL_Material
{
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Shininess;
};

struct Material
{
    std::string Name;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Shininess;
    std::string path_to_texture;

};

struct Mesh
{
    std::string Name;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec3> UV_Coords;
    int MaterialIndex;

    std::vector<glm::vec3> Dummy_Colors;
};

class _3DModelLoader{
 public:
    _3DModelLoader(){};
    void load_3d_object(std::string test_object_file);
    void copyUVCoords(std::vector<glm::vec3>& vertices);
    bool copyVertices(std::vector<glm::vec3>& vertices);
    bool copyNormals(std::vector<glm::vec3>& normals);
    bool copyMaterials(std::vector<OpenGL_Material>& materials);
    bool copyMaterials(std::vector<Material>& materials);
    bool copyColours(std::vector<glm::vec3>& colours);

 private:
    const aiScene *m_scene_;
    void LoadMaterials();
    void LoadMeshes();
    OpenGL_Material* ConvertToOpenGLMaterial(Material* material);
    Material* ProcessMaterial(aiMaterial* material);
    Mesh* ProcessMesh(aiMesh* material);

    Material* CreateRandomMaterial();

    std::vector<Mesh*> meshes_;
    std::vector<Material*> materials_;

    bool has_normals;
    bool has_materials;
    bool has_textures;

};


}

#endif
