#include "loader.h"


namespace loader{

void load_3d_object(std::string test_object_file, std::vector<glm::vec3>& opengl_vertices)
{
  Assimp::Importer importer;
  const aiScene *m_scene = importer.ReadFile(test_object_file,aiProcess_Triangulate | aiProcess_SortByPType |
                                                      aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);


  if( !m_scene)
  {
    std::cout << importer.GetErrorString() << std::endl;
    exit(1);
  }
  if (m_scene->HasMeshes()){
      aiMesh* first_mesh = m_scene->mMeshes[0];
      aiVector3D* vertices = first_mesh->mVertices;

      for (uint i = 0; i < first_mesh->mNumVertices; i++){
          aiVector3t<float> vector = vertices[i];
          glm::vec3 next_vertex(vector.x, vector.y, vector.z);
          opengl_vertices.push_back(next_vertex);

      }

  } else{
      std::cout << "No meshes in scene!" << std::endl;
  }

  if (m_scene->HasMaterials()){
      aiMaterial* first_material = m_scene->mMaterials[0];
  } else{
      std::cout << "No materials in scene!" << std::endl;
  }
}


}
