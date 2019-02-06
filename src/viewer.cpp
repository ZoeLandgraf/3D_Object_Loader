#include "viewer.h"


namespace viewer{
void view(std::vector<glm::vec3> vertices){

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

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


    std::vector<glm::vec3> colours;
    for (uint i = 0; i < vertices.size(); i++){
        colours.push_back(glm::vec3(1.0,0,0));
    }

    //Create and Compile the shaders
    GLuint programID = LoadShaders("../shaders/simple_vertex_shader.vertexshader", "../shaders/simple_fragment_shader.fragmentshader");

    // Pass model to OpenGL
    // This is where th Vertex Array Object VAO has to be created. After creating the contextr and before any other GL call
    //Create VAO and set it as the current one
    GLuint VertexArrayID;
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // identify our vertexbuffers
    GLuint vertexbuffer_model;
    // Generate 1 buffer and use our variable for it
    glGenBuffers(1, &vertexbuffer_model);
    //Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_model);
    //pass the vertexdata
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // colourbuffers
    GLuint colourbuffer_model;
    glGenBuffers(1,&colourbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, colourbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec3), &colours[0], GL_STATIC_DRAW);


    //Compute the transformation matrix
    // This gets a handle for our MVP uniforms
    GLint MatrixID = glGetUniformLocation(programID, "MVP");
    float near_clipping_plane = 0.1f;
    float far_clipping_plane = 100.0f;
    float aspect_ratio = 4.0f/3.0f;
    float FoV = 45.0f;
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, near_clipping_plane, far_clipping_plane);
    glm::mat4 View = glm::lookAt(
                  glm::vec3(2,2,2),  // camera is at 4,3,3 in world space
                  glm::vec3(0,0,0),  // looks at the origin
                  glm::vec3(0,1,0)   // Head is up (set  to (0, -1, 0) to look upside down
                  );
    glm::mat4 mvp = ProjectionMatrix * View;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        //This clears the buffers for colour and depth
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        /* **********draw the cube***** */
        //This send the transformation to the currently bound shader
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        /********* configuring the buffers*******/
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_model);
        glVertexAttribPointer(
                    0,  // must match the layout in the shader
                    3, //size ( how many vertices to draw
                    GL_FLOAT,  // type
                    GL_FALSE,  //normalized?
                    0,         //stride
                    (void*)0   // arraybuffer offset
                    );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colourbuffer_model);
        glVertexAttribPointer(
                    1,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );

        //This won't draw without a shader!!!
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Starting from vertex 0, 3 vertices in total
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();


    }while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

}

    }
