#include <stdio.h>
#include <stdlib.h>

#include <cuda_stdint.h>
#include <vector>
#include <string>

// This has to always be added before glfw3
#include <GL/glew.h>
// This handles the window and the keyboard
#include <GLFW/glfw3.h>


#include <iostream>
// This is a library for 3D mathematics
#include <glm/glm.hpp>

//GLM gtc matrix transform
#include <glm/gtc/matrix_transform.hpp>


#include "loader.h"
#include "viewer.h"
#include "object.h"



int main(){


    // Load object in assimp    
    std::string test_object_file = "/home/zoe/2c6493d31849c15a75f1efca97bee05a/models/model_normalized.obj";

    object::_3D_OG_Object object = object::_3D_OG_Object();

    loader::_3DModelLoader loader = loader::_3DModelLoader("/home/zoe/2c6493d31849c15a75f1efca97bee05a/");
    loader.load_3d_object(test_object_file);

    //initialize glfw
    glewExperimental = true;
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(-1);
    }


    //Some settings
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing which means that we have 4 samples (fragment shader is executed for every sample) per pixel
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); // to define that we want to use Opnegl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // this stops us from using the old Opengl

    //Cretae and open a window // Context creation
    GLFWwindow* window;
    window = glfwCreateWindow(1024,768, "Tutorial 01", NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "Failed to open GLFW window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window); //initializes GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK){
        fprintf(stderr, "Faile to initialze GLEW\n");
        exit(-1);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    object.AddObject(loader.GetMeshes(), loader.GetMaterials());

    // In case of only textures and missing normals, extract uv_coordinates and vertices
    viewer::view(window, object);

    return(0);
}
