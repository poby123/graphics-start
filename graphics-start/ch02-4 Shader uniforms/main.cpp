//
//  main.cpp
//  graphics-start
//
//  Created by wonju Lee on 2023/09/29.
//

#include "common-gl.h"

unsigned int myInitShader();

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
    
    const unsigned int indices[] = {
        0, 1, 2
    };
    
    
    unsigned int shaderProgram = myInitShader();
    
    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind array first, and configure attributes
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // VBO is already registered by glVertexAttribPointer so we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // It is called when window is resized.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    while (!glfwWindowShouldClose(window))
    {
        handleEscOnClose(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        /**
            Insert uniform variable ourColor
         */
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        // set uniform with value
        // * this should be after glUserProgram
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        // glBindVertexArray(0); // no need to unbind it every time
        
        // double buffering & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}



unsigned int myInitShader() {
    /**
        Vertex Shader
     */
    // vertex shader in GLSL (OpenGL Shading Language)
    const char* vertextShaderSource = "                 \
    #version 330 core                                   \
    layout (location = 0) in vec3 aPos;                 \
                                                        \
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
    #version 330 core                             \
    out vec4 FragColor;                           \
                                                  \
    uniform vec4 ourColor;                        \
                                                  \
    void main()                                   \
    {                                             \
        FragColor = ourColor;                     \
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
    
    return shaderProgram;
}
