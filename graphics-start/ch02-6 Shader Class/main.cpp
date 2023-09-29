//
//  main.cpp
//  graphics-start
//
//  Created by wonju Lee on 2023/09/29.
//

#include "common-gl.h"
#include <my/shader_s.h>

const std::string projectPath = "/Users/wonjulee/Desktop/workspace/study/graphics-start/graphics-start/ch02-6 Shader Class/";
const std::string vertexShaderPath = std::string(projectPath + "shader.vs");
const std::string fragmentShaderPath = std::string(projectPath + "shader.fs");

int main()
{
    GLFWwindow* window = myOpenGLInit(800, 600);
    if(window == NULL){
        glfwTerminate();
        return -1;
    }
    
    const float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,   0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    
    const unsigned int indices[] = {
        0, 1, 2
    };
    
    Shader ourShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    
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
    
    
    // position attributes: layout 0 in GLSL, stride: 6(x, y, z, r, g, b) * 4, offset: 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attributes: layout 1 in GLSL, stride: 6(x, y, z, r, g, b) * 4, offset: 3(x, y, z) * 4
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // VBO is already registered by glVertexAttribPointer so we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        // double buffering & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}
