#include "loader.h"


namespace loader{


Material* _3DModelLoader::CreateRandomMaterial(){

    Material* material_info = new Material;
    material_info->Ambient = glm::vec3(0.0, 0.0, 0.0);
    material_info->Diffuse = glm::vec3(0.64, 0.48, 0.32);
    material_info->Specular = glm::vec3(0.5, 0.5, 0.5);
    material_info->Shininess = 96;

    return material_info;
}


void _3DModelLoader::LoadMaterials(){

    if (m_scene_->HasMaterials()){
        for (int i = 0; i < m_scene_->mNumMaterials; i++){
            materials_.push_back(ProcessMaterial(m_scene_->mMaterials[i]));
            has_materials_ = true;
        }
    }
    else{
        std::cout<<"No materials found" << std::endl;
        has_materials_ = false;
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

    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
       aiString Path;
       if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

           boost::filesystem::path tp = Path.data;
           boost::filesystem::path base_dir = boost::filesystem::path(path_to_model_);
           for(auto& part : tp){
               if (!((part.string()) == "..")){
                   base_dir /= part;
               }
           }

           material_info->PathToTexture = base_dir.string();
           material_info->hasTexture = true;
       } else{

           material_info->PathToTexture = "";
           material_info->hasTexture = false;
       }
    } else {
        material_info->hasTexture = false;
    }
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
        mesh_info->hasNormals = true;
    }else{
        std::cout<<"Mehs has no normals!"<<std::endl;
        mesh_info->hasNormals = false;
    }

    if (mesh->HasTextureCoords(0)){
        for (int i = 0; i < mesh->mNumVertices; i++){
        //for (int i = (mesh->mNumVertices) - 1; i >= 0; i--){
            aiVector3t<float> uv_coords = mesh->mTextureCoords[0][i];
            glm::vec2 next_uv_coord = glm::vec2(uv_coords.x, uv_coords.y);
            mesh_info->UV_Coords.push_back(next_uv_coord);
            mesh_info->hasUVCoords  = true;
        }
    } else {
            std::cout<<"Mehs has no uv_coords!!!"<<std::endl;
             mesh_info->hasUVCoords  = false;
    }

    Dummy_Colours dummy_colours;
    //Generate a random colour for this mesh
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // add dummy_colours
    for (uint i = 0; i < mesh->mNumVertices; i++){
        glm::vec3 random_colour = glm::vec3(r,g,b);
        mesh_info->DummyColors.push_back(random_colour);
        mesh_info->hasDummyColours  = true;
    }


    mesh_info->MaterialIndex = mesh->mMaterialIndex;


    return mesh_info;
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
  m_scene_ = importer.ReadFile(test_object_file,aiProcess_Triangulate | aiProcess_SortByPType);
  if( !m_scene_)
  {
    std::cout << importer.GetErrorString() << std::endl;
    exit(1);
  }

  LoadMaterials();
  LoadMeshes();
}



std::vector<Mesh*> _3DModelLoader::GetMeshes(){
    return meshes_;
}
std::vector<Material*> _3DModelLoader::GetMaterials(){
    return materials_;
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
