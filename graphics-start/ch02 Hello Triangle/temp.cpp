//
//  main.cpp
//  graphics-start
//
//  Created by wonju Lee on 2023/09/28.
//

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
using namespace std;

void checkShaderCompile(unsigned int shader);

int main()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    
    // to manage graphic card memory, using vertex buffer objects(VBO)
    // VBO has a unique ID.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    // bind the created buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // copy user-defined data into currently bound buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
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
    version 330 core                              \
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
    
    
    return 0;
}

void checkShaderCompile(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if(success){
        return;
    }
    
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::COMPILEATION_FAILED\n" << infoLog << endl;
}
