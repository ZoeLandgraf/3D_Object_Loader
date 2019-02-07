#include "object.h"


namespace object{



void _3D_OG_Mesh::setColours(std::vector<glm::vec3> colours){
    // Generate 1 buffer and use our variable for it
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec3), &colours[0], GL_STATIC_DRAW);
    vertices_ = colours;
}


void _3D_OG_Mesh::setVertices(std::vector<glm::vec3> vertices){

    // Generate 1 buffer and use our variable for it
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    vertices_ = vertices;

}

void _3D_OG_Mesh::setNormals(std::vector<glm::vec3> normals){

    // Generate 1 buffer and use our variable for it
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    normals_ = normals;

}

void _3D_OG_Mesh::setUVCoords(std::vector<glm::vec3> uv_coords){

    // Generate 1 buffer and use our variable for it
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uv_coords.size() * sizeof(glm::vec3), &uv_coords[0], GL_STATIC_DRAW);
    uv_coords_ = uv_coords;

}

void _3D_OG_Mesh::setTexture(std::string texture_file){
    // Create one OpenGL texture

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }
    glGenTextures(1, &Texture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, Texture);
    //set the inetrpolation setting -> if uv coordinate does not exactly fall on a pixel centre
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //check the file exists

    //load teh image
    const char* tf = texture_file.c_str();
    int w, h;
    cv::Mat image;
    image = cv::imread(tf, CV_LOAD_IMAGE_COLOR);
    w = image.cols;
    h = image.rows;

    unsigned char* loaded_image = SOIL_load_image(tf, &w, &h, 0, SOIL_LOAD_AUTO);
//    *texture_image = SOIL_load_OGL_texture(tf, SOIL_LOAD_AUTO, // RGB
//                              SOIL_CREATE_NEW_ID, // overwriting old texture
//                              SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);

    if(*loaded_image == NULL){
            printf("[Texture loader] \"%s\" failed to load!\n", tf);
    // Give the image to OpenGL
    }
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, loaded_image);

//    SOIL_free_image_data(texture_image);

}


std::vector<glm::vec3> _3D_OG_Mesh::getVertices(){
    return vertices_;
}

GLuint _3D_OG_Mesh::getVertexBuffer(){
    return vertexBuffer;
}

GLuint _3D_OG_Mesh::getTexture(){
    return Texture;
}

GLuint _3D_OG_Mesh::getColourBuffer(){
    return colourBuffer;
}

GLuint _3D_OG_Mesh::getUVBuffer(){
    return uvBuffer;
}

_3D_OG_Mesh::_3D_OG_Mesh(loader::Mesh* mesh, loader::Material* material){

    //set vertexbuffer
    assert(mesh->Vertices.size() >0);
    setVertices(mesh->Vertices);

    //set Normalbuffer
    if (!(mesh->Normals).empty()){
        setNormals(mesh->Normals);
        has_normals = true;
    }
    //set Uv buffer
    if(!(mesh->UV_Coords).empty()){
            setUVCoords(mesh->UV_Coords);
          has_uv_coords = true;

          if(material->path_to_texture == ""){
              has_texture = false;
          }else{
              setTexture(material->path_to_texture);
               has_texture = true;
          }

     }

}

_3D_OG_Object::_3D_OG_Object(){};

std::vector<_3D_OG_Mesh> _3D_OG_Object::GetMeshes(){
    return meshes_;
}

void _3D_OG_Object::AddObject(std::vector<loader::Mesh*> meshes, std::vector<loader::Material*> materials){

    //assert(meshes.size() == materials.size());
    for (int i = 0; i < meshes.size(); i++)
    {
        int material_index = meshes[i]->MaterialIndex;
        _3D_OG_Mesh mesh = _3D_OG_Mesh(meshes[i], materials[material_index]);
        meshes_.push_back(mesh);
    }


}

}


