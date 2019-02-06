#include "loader.h"


namespace loader{



void _3DModelLoader::LoadMaterials(){

    if (m_scene_->HasMaterials()){
        for (int i = 0; i < m_scene_->mNumMaterials; i++){
            materials_.push_back(ProcessMaterial(m_scene_->mMaterials[i]));
        }
    }
    else{
        std::cout<<"No materials" << std::endl;
    }
}


Material* _3DModelLoader::ProcessMaterial(aiMaterial* material){
    Material* material_info = new Material;
    aiString mname;
    material->Get(AI_MATKEY_NAME, mname);
    material_info->Name = mname.C_Str();

    int shadingModel;
    material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

    aiColor3D dif(0.f,0.f,0.f);
    aiColor3D amb(0.f,0.f,0.f);
    aiColor3D spec(0.f,0.f,0.f);
    float shine = 0.0;

    material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
    material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
    material->Get(AI_MATKEY_SHININESS, shine);

    material_info->Ambient = glm::vec3(amb.r, amb.g, amb.b);
    material_info->Diffuse = glm::vec3(dif.r, dif.g, dif.b);
    material_info->Specular = glm::vec3(spec.r, spec.g, spec.b);
    material_info->Shininess = shine;

//           mater-&gt;Ambient *= .2;
//           if (mater-&gt;Shininess == 0.0)
//               mater-&gt;Shininess = 30;

    return material_info;

}


Mesh* _3DModelLoader::ProcessMesh(aiMesh* mesh){

    Mesh* mesh_info = new Mesh;
    for (uint i = 0; i < mesh->mNumVertices; i++){
        aiVector3t<float> vector = mesh->mVertices[i];
        glm::vec3 next_vertex = glm::vec3(vector.x, vector.y, vector.z);
        mesh_info->Vertices.push_back(next_vertex);
    }
    if (mesh->HasNormals()){
        for (uint i = 0; i < mesh->mNumVertices; i++){
            aiVector3t<float> normal = mesh->mNormals[i];
            glm::vec3 next_normal = glm::vec3(normal.x, normal.y, normal.z);
            mesh_info->Normals.push_back(next_normal);
        }
    }

    mesh_info->MaterialIndex = mesh->mMaterialIndex;
    //TODO: Normals
}

void _3DModelLoader::LoadMeshes(){

    if (m_scene_->HasMeshes()){
        for (int i = 0; i < m_scene_->mNumMeshes; i++){

            meshes_.push_back(ProcessMesh(m_scene_->mMeshes[i]));
        }
    }else{
        std::cout<<"No meshes"<<std::endl;
    }

}

void _3DModelLoader::load_3d_object(std::string test_object_file)
{
  Assimp::Importer importer;
  m_scene_ = importer.ReadFile(test_object_file,aiProcess_Triangulate | aiProcess_SortByPType |
                                                      aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
  if( !m_scene_)
  {
    std::cout << importer.GetErrorString() << std::endl;
    exit(1);
  }

  LoadMaterials();
  LoadMeshes();
}


void _3DModelLoader::copyVertices(std::vector<glm::vec3>& vertices){
    for(uint m = 0; m < meshes_.size(); m++){

        for(int i = 0; i < (meshes_[m])->Vertices.size(); i ++){
            vertices.push_back(meshes_[m]->Vertices[i]);
    }

    }
}

void _3DModelLoader::copyNormals(std::vector<glm::vec3>& normals){
    for(uint m = 0; m < meshes_.size(); m++){

        for(int i = 0; i < (meshes_[m])->Normals.size(); i ++){
            normals.push_back(meshes_[m]->Normals[i]);
    }

    }
}

void _3DModelLoader::copyMaterials(std::vector<OpenGL_Material>& materials){
    for(uint m = 0; m < meshes_.size(); m++){
        uint Material_index = meshes_[m]->MaterialIndex;
        for(uint i = 0; i < (meshes_[m])->Vertices.size(); i ++){
            materials.push_back(*ConvertToOpenGLMaterial((materials_[Material_index])));
    }
    }
}

OpenGL_Material* _3DModelLoader::ConvertToOpenGLMaterial(Material* material){
    OpenGL_Material* open_gl_material = new OpenGL_Material;
    open_gl_material->Ambient = material->Ambient;
    open_gl_material->Diffuse = material->Diffuse;
    open_gl_material->Specular = material->Specular;
    open_gl_material->Shininess = material->Shininess;

    return open_gl_material;
}

}
