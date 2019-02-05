#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <stdlib.h>

// This has to always be added before glfw3
#include <GL/glew.h>
// This handles the window and the keyboard
#include <GLFW/glfw3.h>

// This is a library for 3D mathematics
#include <glm/glm.hpp>


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
