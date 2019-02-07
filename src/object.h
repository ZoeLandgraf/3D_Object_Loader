#ifndef OBJECT_H
#define OBJECT_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <vector>


#include "common/shader.hpp"

//GLM gtc matrix transform
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "SOIL.h"
#include <GLFW/glfw3.h>
#include "loader.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>

namespace object {



class _3D_OG_Mesh{
public:
    _3D_OG_Mesh(loader::Mesh* mesh, loader::Material* material);

    GLuint getVertexBuffer();
    GLuint getNormalBuffer();
    GLuint getUVBuffer();
    GLuint getColourBuffer();
    GLuint getTexture();

    std::vector<glm::vec3> getVertices();
    bool has_uv_coords;
    bool has_normals;
    bool has_texture;

    //TODO: set material buffer

private:
    GLuint vertexBuffer;
    GLuint colourBuffer;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint Texture;

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec3> uv_coords_;
    std::vector<glm::vec3> colours_;
    unsigned char* texture_image;




    void setVertices(std::vector<glm::vec3> vertices);
    void setNormals(std::vector<glm::vec3> normals);
    void setUVCoords(std::vector<glm::vec3> uv_coords);
    void setColours(std::vector<glm::vec3> colours);
    void setTexture(std::string texture_file);

};


class _3D_OG_Object{
  public:
    _3D_OG_Object();
    void AddObject(std::vector<loader::Mesh*> meshes, std::vector<loader::Material*> materials);
    std::vector<_3D_OG_Mesh> GetMeshes();
  private:
    std::vector<_3D_OG_Mesh> meshes_;

};


}

#endif // OBJECT_H


