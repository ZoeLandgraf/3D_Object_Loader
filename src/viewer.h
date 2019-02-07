#ifndef VIEWER_H
#define VIEWER_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "common/shader.hpp"

//GLM gtc matrix transform
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "control.h"
#include "loader.h"
#include "object.h"

namespace viewer{
  void view(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
            std::vector<loader::OpenGL_Material> materials,
            std::vector<glm::vec3> colours);

  void view(std::vector<glm::vec3> vertices,
            std::vector<glm::vec3> colours);

  void view(GLFWwindow* window, object::_3D_OG_Object& object);
}

#endif //VIEWER_H
