# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (LoadShapeNetObjects)
set (CMAKE_CXX_STANDARD 11)
find_package(OpenGL REQUIRED)
FIND_PACKAGE(ASSIMP REQUIRED)

FIND_LIBRARY(${SOIL_LIBRARY} NAMES libSOIL.a
    PATHS "/home/zoe/")


#----------------------------------------------------------
# Dependencies - OpenCV
#----------------------------------------------------------
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()


include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        commmon/
        ${ASSIMP_INCLUDE}
        ${PATH_TO_SOIL}
        /home/zoe/Installations/Simple_OpenGL_Image_Library/src/
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)


set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        ${OpenCV_LIBS}
        -lassimp
        -lSOIL
        -lboost_system -lboost_filesystem
        #-opencv_core
        #-opencv_highgui
        #-opencv_imgproc
        #-opencv_imgcodecs
)

# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

#Load shapenet objects
add_executable(shapenetloader
    common/shader.cpp
    loader.cpp
    viewer.cpp
    main.cpp
    control.cpp
    object.cpp
    shaders/simple_vertex_shader.vertexshader
    shaders/simple_fragment_shader.fragmentshader
    shaders/material_vertex_shader.vertexshader
    shaders/material_fragment_shader.fragmentshader
    shaders/texture_vertex_shader.vertexshader
    shaders/texture_fragment_shader.fragmentshader
    )


target_link_libraries(shapenetloader
        ${ALL_LIBS}

)
#set_target_properties(shapenetloader PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shapenet_loader/")
#create_target_launcher(shapenetloader WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shapenet_loader/")




SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
