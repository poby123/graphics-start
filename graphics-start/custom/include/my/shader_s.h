//
//  shader_s.hpp
//  graphics-start
//
//  Created by wonju Lee on 2023/09/29.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    enum class CHECK_TYPE { PROGRAM, VERTEX, FRAGMENT };
    void checkCompileErrors(unsigned int shader, CHECK_TYPE type);
public:
    
    // The Program ID
    unsigned int ID;
    
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // activate the shader
    void use();
    
    // utilify uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    friend std::ostream& operator<<(std::ostream &out, const CHECK_TYPE type) {
        std::string typeString = "";
        switch (type) {
            case CHECK_TYPE::PROGRAM: typeString = "PROGRAM"; break;
            case CHECK_TYPE::VERTEX: typeString = "PROGRAM"; break;
            case CHECK_TYPE::FRAGMENT: typeString = "PROGRAM"; break;
            default:
                break;
        }
        
        out << typeString;
        
        return out;
    };
};


void Shader::checkCompileErrors(unsigned int shader, CHECK_TYPE type)
{
    int success;
    char infoLog[1024];
    
    if (type == CHECK_TYPE::VERTEX || type == CHECK_TYPE::FRAGMENT)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success)
        {
            return;
        }
        
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
    else if(type == CHECK_TYPE::PROGRAM)
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success)
        {
            return;
        }
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the source code from filepath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ensure objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        std::cout << "current path: " << std::filesystem::current_path() << std::endl;
        std::cout << "vertexPath: " << vertexPath << ", fragmentPath: " << fragmentPath << std::endl;
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        std::cout << e.what() << std::endl;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, CHECK_TYPE::VERTEX);
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, CHECK_TYPE::FRAGMENT);
    
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, CHECK_TYPE::PROGRAM);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// activate the shader
void Shader::use()
{
    glUseProgram(ID);
}

// utilify uniform functions
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


#endif /* shader_s_hpp */
