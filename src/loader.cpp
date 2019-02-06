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
            //materials_.push_back(CreateRandomMaterial());
            has_materials = true;
        }
    }
    else{
        std::cout<<"No materials" << std::endl;
        has_materials = false;
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
           material_info->path_to_texture = Path.data;
           has_textures = true;
       } else{

           material_info->path_to_texture = "";
           has_textures = false;
       }
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
        has_normals = true;
    }else{
        std::cout<<"Mehs has no normals!"<<std::endl;
        has_normals = false;
    }

    if (mesh->HasTextureCoords(0)){
        for (uint i = 0; i < mesh->mNumVertices; i++){
            aiVector3t<float> uv_coords = mesh->mTextureCoords[0][i];
            glm::vec3 next_uv_coord = glm::vec3(uv_coords.x, uv_coords.y, uv_coords.z);
            mesh_info->UV_Coords.push_back(next_uv_coord);
        }
    } else {
            std::cout<<"Mehs has no uv_coords!!!"<<std::endl;
        }

    Dummy_Colours dummy_colours;
    int random_colour_id = rand() % (dummy_colours.colour_array.size());
    std::cout<<" random_colour for mesh "<< random_colour_id << std::endl;
    // add dummy_colours
    for (uint i = 0; i < mesh->mNumVertices; i++){
        glm::vec3 random_colour = glm::vec3(dummy_colours.colour_array[random_colour_id]);
        mesh_info->Dummy_Colors.push_back(random_colour);
    }


    mesh_info->MaterialIndex = mesh->mMaterialIndex;
    //TODO: Normals

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


bool _3DModelLoader::copyVertices(std::vector<glm::vec3>& vertices){
    for(uint m = 0; m < meshes_.size(); m++){

        for(int i = 0; i < (meshes_[m])->Vertices.size(); i++){
            vertices.push_back(meshes_[m]->Vertices[i]);           
    }

    }
    return true;
}

bool _3DModelLoader::copyNormals(std::vector<glm::vec3>& normals){

    if (!has_normals){
        return false;
    }

    for(uint m = 0; m < meshes_.size(); m++){

        for(int i = 0; i < (meshes_[m])->Normals.size(); i++){
            normals.push_back(meshes_[m]->Normals[i]);
        }

    }
    return true;
}

bool _3DModelLoader::copyMaterials(std::vector<OpenGL_Material>& materials){

    if (!has_materials){
        return false;
    }
    for(uint m = 0; m < meshes_.size(); m++){
        uint Material_index = meshes_[m]->MaterialIndex;
        for(uint i = 0; i < (meshes_[m])->Vertices.size(); i++){
            materials.push_back(*ConvertToOpenGLMaterial((materials_[Material_index])));
    }
    }
    return true;
}

bool _3DModelLoader::copyColours(std::vector<glm::vec3>& colours){
    for(uint m = 0; m < meshes_.size(); m++){

        for(int i = 0; i < (meshes_[m])->Dummy_Colors.size(); i ++){
            colours.push_back(meshes_[m]->Dummy_Colors[i]);
        }
    }
    return true;
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
