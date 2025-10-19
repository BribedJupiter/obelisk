#include "obShader.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string loadShaders(const std::string filename) {
    std::ifstream file;
    file.exceptions(file.exceptions() | std::ios::failbit | std::ios::badbit);
    try {
        file.open(filename.c_str());
    } catch (std::exception e) {
        std::cerr << "ERROR::SHADER::FILE_READ_FAILURE -->" << filename  << " -> " << e.what() << std::endl;
        return "";
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return text;
}

Shader::Shader(const std::string vertexPath, const std::string fragmentPath) {
    // See LearnOpenGL's section on shaders: https://learnopengl.com/Getting-started/Shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Load Shaders
    const std::filesystem::path shaderSourceDir = SHADER_PATH;
    std::string vertexShaderString = loadShaders(shaderSourceDir.string() + vertexPath);
    std::string fragmentShaderString = loadShaders(shaderSourceDir.string() + fragmentPath);
    if (vertexShaderString == "" || fragmentShaderString == "") {
        std::cerr << "ERROR::SHADER::FAILED_SHADER_LOAD" << std::endl;
    }
    const char* vertexShaderSource = vertexShaderString.c_str();
    const char* fragmentShaderSource = fragmentShaderString.c_str();

    // Compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);

    // Check if vertex shader compilation was successful
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    fragment= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);

    // Check if fragment shader compilation was successful
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader program setup
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Check for shader program success
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    // Delete the now unneeded (after linking) shader objects
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

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
void Shader::setMat4(const std::string &name, glm::mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
} 