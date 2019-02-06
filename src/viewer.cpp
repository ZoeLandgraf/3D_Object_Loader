#include "viewer.h"


namespace viewer{
void view(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<loader::OpenGL_Material> materials){

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

    // normalbuffer
    GLuint normalbuffer_model;
    glGenBuffers(1,&normalbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


    std::vector<glm::vec3> Ka;
    std::vector<glm::vec3> Kd;
    std::vector<glm::vec3> Ks;
    std::vector<glm::float1> S;
    for( loader::OpenGL_Material material : materials){
        Ka.push_back(material.Ambient);
        Kd.push_back(material.Diffuse);
        Ks.push_back(material.Specular);
        S.push_back(material.Shininess);
    }


    // KAbuffer
    GLuint KAbuffer_model;
    glGenBuffers(1,&KAbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, KAbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, Ka.size() * sizeof(glm::vec3), &Ka[0], GL_STATIC_DRAW);

    // KDbuffer
    GLuint KDbuffer_model;
    glGenBuffers(1,&KDbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, KDbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, Kd.size() * sizeof(glm::vec3), &Kd[0], GL_STATIC_DRAW);

    // KSbuffer
    GLuint KSbuffer_model;
    glGenBuffers(1,&KSbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, KSbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, Ks.size() * sizeof(glm::vec3), &Ks[0], GL_STATIC_DRAW);

    // Sbuffer
    GLuint Sbuffer_model;
    glGenBuffers(1,&Sbuffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, Sbuffer_model);
    glBufferData(GL_ARRAY_BUFFER, S.size() * sizeof(glm::float1), &S[0], GL_STATIC_DRAW);

    //Get uniform handels
    GLint MVP_ID = glGetUniformLocation(programID, "MVP");
    GLint MV_ID = glGetUniformLocation(programID, "MV");
    GLint N_ID = glGetUniformLocation(programID, "N");


    GLint KA_ID = glGetUniformLocation(programID, "Material[0].Type");
    GLint KD_ID = glGetUniformLocation(programID, "Material[1].Type");
    GLint KS_ID = glGetUniformLocation(programID, "Material[2].Type");
    GLint S_ID = glGetUniformLocation(programID, "Material[3].Type");


    GLint LIGHT_POSITION_ID = glGetUniformLocation(programID, "Light[0].Type");
    GLint LIGHT_INTENSITY_ID = glGetUniformLocation(programID, "Light[1].Type");

    //Compute the transformation matrix


    float near_clipping_plane = 0.1f;
    float far_clipping_plane = 100.0f;
    float aspect_ratio = 4.0f/3.0f;
    float FoV = 45.0f;
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, near_clipping_plane, far_clipping_plane);
    glm::mat4 View = glm::lookAt(
                  glm::vec3(-1,-1,-2),  // camera is at 4,3,3 in world space
                  glm::vec3(0,0,0),  // looks at the origin
                  glm::vec3(0,-1,0)   // Head is up (set  to (0, -1, 0) to look upside down
                  );
    glm::mat4 mvp = ProjectionMatrix * View;
    glm::mat4 mv = View;

    glm::mat3x3 modelMatrix3(mv);
    glm::mat3x3 n = glm::inverseTranspose(modelMatrix3);


    glm::float3 light_intensity(1.0,1.0,1.0);
    glm::float4 light_position(-1.0,1.0,1.0,1.0);


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        //This clears the buffers for colour and depth
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        /* **********draw the cube***** */
        //This send the transformation to the currently bound shader
        glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(MV_ID, 1, GL_FALSE, &mv[0][0]);
        glUniformMatrix3fv(N_ID, 1, GL_FALSE, &n[0][0]);

        glUniform4f(LIGHT_POSITION_ID, light_position[0],light_position[1],light_position[2],light_position[3]);
        glUniform3f(LIGHT_INTENSITY_ID, light_intensity[0],light_intensity[1],light_intensity[2]);

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
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_model);
        glVertexAttribPointer(
                    1,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, KAbuffer_model);
        glVertexAttribPointer(
                    2,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, KDbuffer_model);
        glVertexAttribPointer(
                    3,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, KSbuffer_model);
        glVertexAttribPointer(
                    4,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );
        glEnableVertexAttribArray(5);
        glBindBuffer(GL_ARRAY_BUFFER, Sbuffer_model);
        glVertexAttribPointer(
                    5,
                    1,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );


        //This won't draw without a shader!!!
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Starting from vertex 0, 3 vertices in total
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);


        glfwSwapBuffers(window);
        glfwPollEvents();


    }while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

}

    }
