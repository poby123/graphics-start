//
//  main.cpp
//  graphics-start
//
//  Created by wonju Lee on 2023/09/28.
//

#include "common-gl.h"

int main()
{
    GLFWwindow* window = myOpenGLInit(800, 600);
    if(window == NULL){
        glfwTerminate();
        return -1;
    }
    
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    
    /**
        Vertex Shader
     */
    // vertex shader in GLSL (OpenGL Shading Language)
    const char* vertextShaderSource = "                 \
    #version 330 core                                   \
    layout (location = 0) in vec3 aPos;                 \
    void main()                                         \
    {                                                   \
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \
    }                                                   \
    \0";
    
    // compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader);
    
    
    /**
        Fragment Shader
     */
    const char* fragmentShaderSource = "          \
    #version 330 core                              \
    out vec4 FragColor;                           \
                                                  \
    void main()                                   \
    {                                             \
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \
    }                                             \
    \0";
    
    // compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader);
    
    
    /**
        Shader Program
        that is the final liked version of multiple shaders combined.
     */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLink(shaderProgram);
    
    // once shaders are attached the program, they should be deleted.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    
    /**
        VBO (Vertex Buffer Objects)
        VAO (Vertex Array Objects)
     */
    unsigned int VBO, VAO;
    
    // create & bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // 1. copy our vertecies array in a buffer for OpenGL to use
    // * this should be done before glVertexAttribPointer which use GL_ARRAY_BUFFER
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    // 2. then set the vertex attributes pointers.
    //      - 0: vertex shader, location = 0
    //      - 3: vertex shader, vec3
    //      - GL_FLOAT: vertex shader data type
    //      - GL_FALSE: option for normalizing if want set GL_TRUE
    //      - 3 * sizeof(float): stride
    //      - (void*)0: offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // VBO is already registered by glVertexAttribPointer so we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
    
    // It is called when window is resized.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    while (!glfwWindowShouldClose(window))
    {
        handleEscOnClose(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangle
        glUseProgram(shaderProgram);
        
        // seeing as we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
        
        
        // double buffering & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}
