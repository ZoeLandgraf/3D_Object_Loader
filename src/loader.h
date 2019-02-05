#include <stdio.h>
#include <stdlib.h>

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

#include "viewer.h"


namespace loader{



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

 private:
    const aiScene *m_scene_;
    void LoadMaterials();
    void LoadMeshes();
    Material* ProcessMaterial(aiMaterial* material);
    Mesh* ProcessMesh(aiMesh* material);

    std::vector<Mesh*> meshes_;
    std::vector<Material*> materials_;

};


}
