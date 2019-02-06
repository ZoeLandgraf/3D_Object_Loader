
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
};

struct Mesh
{
    std::string Name;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec3> Normals;
    int MaterialIndex;
};

class _3DModelLoader{
 public:
    _3DModelLoader(){};
    void load_3d_object(std::string test_object_file);
    void copyVertices(std::vector<glm::vec3>& vertices);
    void copyNormals(std::vector<glm::vec3>& normals);
    void copyMaterials(std::vector<OpenGL_Material>& materials);
    void copyMaterials(std::vector<Material>& materials);

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

};


}

#endif
