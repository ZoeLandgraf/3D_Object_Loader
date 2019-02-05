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

int main(){


    //TODO: debug scenenet with a shapenet object which is rednered properly
            //debug it with one which is not rendered nicely -> compare what data looks like after the material has been loaded
    // Load object in assimp
    std::string test_object_file = "/home/zoe/2c6493d31849c15a75f1efca97bee05a/models/model_normalized.obj";

    std::vector<glm::vec3> opengl_vertices;

    loader::_3DModelLoader loader = loader::_3DModelLoader();
    loader.load_3d_object(test_object_file);

    viewer::view(opengl_vertices);

    return(0);
}
