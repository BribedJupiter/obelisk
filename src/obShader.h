#ifndef OBSHADER_H
#define OBSHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>


class Shader {
    public:
        // The shader program's ID
        unsigned int ID;

        // A constructor that will read the shader and compile it
        // Give shader paths relative to the /shaders directory
        Shader(const std::string vertexPath, const std::string fragmentPath);

        // Use and activate the shader
        void use();

        // Utilities for uniforms
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
};

#endif