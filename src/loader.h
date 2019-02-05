#include <stdio.h>
#include <stdlib.h>

#include <cuda_stdint.h>
#include <vector>
#include <string>

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include "viewer.h"


namespace loader{
void load_3d_object(std::string test_object_file, std::vector<glm::vec3>& opengl_vertices);
}
