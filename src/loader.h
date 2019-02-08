
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
#include <boost/filesystem.hpp>





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
    std::string PathToTexture;
    bool hasTexture;

    Material() {hasTexture = false;
               Shininess = 0;
               Name = "";
               PathToTexture = "";
               }

};

struct Mesh
{
    std::string Name;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> UV_Coords;
    int MaterialIndex;
    std::vector<glm::vec3> DummyColors;

    bool hasNormals;
    bool hasUVCoords;
    bool hasDummyColours;

    Mesh(){Name = "";
          MaterialIndex = -1;
          hasNormals = false;
          hasUVCoords = false;
          hasDummyColours = false;
          }
};



class _3DModelLoader{
 public:
    _3DModelLoader(std::string path_to_model):path_to_model_(path_to_model){};
    void load_3d_object(std::string test_object_file);


    std::vector<Mesh*> GetMeshes();
    std::vector<Material*> GetMaterials();

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
    bool has_meshes_;
    bool has_materials_;

    std::string path_to_model_;


};


}

#endif
