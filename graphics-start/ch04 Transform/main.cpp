//
//  main.cpp
//  graphics-start
//
//  Created by wonju Lee on 2023/10/02.
//
#define STB_IMAGE_IMPLEMENTATION

#include "common-gl.h"
#include <my/shader_s.h>
#include <stb-master/stb_image.h>
#include <my/path.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const std::string texturePath = std::string(projectPath + "/resources/textures");
const std::string vertexShaderPath = std::string(srcPath + "/ch04 Transform/shader.vs");
const std::string fragmentShaderPath = std::string(srcPath + "/ch04 Transform/shader.fs");

int main()
{
    GLFWwindow* window = myOpenGLInit(800, 600);
    if(window == NULL){
        glfwTerminate();
        return -1;
    }
    
    float vertices[] = {
        // positions          // colors          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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
    
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    /**
        Textures
     */
    unsigned int texture1, texture2;
    
    // Texture1: load container image texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    
    const std::string containerTexturePath = texturePath + "/container.jpg";
    unsigned char *data = stbi_load(containerTexturePath.c_str(), &width, &height, &nrChannels, 0);
    
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture";
    }
    stbi_image_free(data);
    
    
    // Texture2: load smile image texture
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);
    const std::string smileTexturePath = texturePath + "/awesomeface.png";
    data = stbi_load(smileTexturePath.c_str(), &width, &height, &nrChannels, 0);
    
    if(data)
    {
        // awesomeface.png has an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBAs
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture";
    }
    stbi_image_free(data);
    
    
    ourShader.use(); // activate shader is required before set uniforms.
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    
    
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        
        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
        float timeValue = glfwGetTime();
        float positionY = (sin(timeValue) / 2.0f);
        float positionX = (cos(timeValue) / 2.0f);
        transform = glm::translate(transform, glm::vec3(positionX, positionY, 0.0f));
        transform = glm::rotate(transform, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));
        
        // render container
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
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