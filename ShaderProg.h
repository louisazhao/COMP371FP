//
//  ShaderProg.h
//  COMP371A2
//
//  Created by Zhao Rui on 2018-02-23.
//  Modified from https://learnopengl.com/
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#ifndef ShaderProg_h
#define ShaderProg_h
// GLEW
#define GLEW_STATIC
#include </usr/local/Cellar/glew/2.1.0/include/GL/glew.h>

//GLFW
#include </usr/local/Cellar/glfw-master/include/GLFW/glfw3.h>

//glm
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/glm.hpp>
#include <string>

class ShaderProg{
public:
    unsigned int ID;
    
    ShaderProg(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
    void use();
    void setBoolean(const std::string &name, bool value)const;
    void setInt(const std::string &name, int value)const;
    void setFloat(const std::string &name, float value)const;
    void setVec2(const std::string &name, const glm::vec2 &value)const;
    void setVec2(const std::string &name, float x,float y)const;
    void setVec3(const std::string &name, const glm::vec3 &value)const;
    void setVec3(const std::string &name, float x, float y, float z)const;
    void setVec4(const std::string &name, const glm::vec4 &value)const;
    void setVec4(const std::string &name, float x, float y, float z, float w)const;
    void setMat2(const std::string &name, const glm::mat2 &mat)const;
    void setMat3(const std::string &name, const glm::mat3 &mat)const;
    void setMat4(const std::string &name, const glm::mat4 &mat)const;
    
    
private:
    void checkCompileErrors(GLuint shader, std::string type);
};



#endif /* ShaderProg_h */



