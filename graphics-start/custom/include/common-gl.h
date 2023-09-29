//
//  common-gl.h
//  graphics-start
//
//  Created by wonju Lee on 2023/09/28.
//

#ifndef common_gl_h
#define common_gl_h

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
using namespace std;

void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* myOpenGLInit(unsigned width, unsigned height, const char* title);
void checkShaderCompile(unsigned int shader);
void checkProgramLink(unsigned int shaderProgram);


///


/**
 Default Input Callback function for closing window when user press the ESC key.
 */
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

/**
 Callback function for handling the window resized
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}


/**
 OpenGL initialization
 */
GLFWwindow* myOpenGLInit(unsigned width, unsigned height, const char* title = "Learn OpenGL") {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    
    // GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    return window;
}

/**
 Validation shader string
 */
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

/**
 Validation program linking
 */
void checkProgramLink(unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(success){
        return;
    }
    
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
}



#endif /* common_gl_h */
