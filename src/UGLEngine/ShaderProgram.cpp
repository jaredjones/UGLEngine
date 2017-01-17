//
//  ShaderProgram.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "ShaderProgram.h"
#include <fstream>
#include <gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(std::string vertexFile, std::string fragFile) {
    this->vertexShaderID = LoadShader(vertexFile, GL_VERTEX_SHADER);
    this->fragmentShaderID = LoadShader(fragFile, GL_FRAGMENT_SHADER);
    
    // Link the program
    fprintf(stdout, "Linking shader program\n");
    this->programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
}

void ShaderProgram::LinkShader() {
    //All attributes must be bound before the program is linked
    BindAttributes();
    glLinkProgram(programID);
    
    //Note: Since this validates the program for the given OpenGL state, this will claim that no VAO is bound
    //glValidateProgram(programID);
    
    GLint result = GL_FALSE;
    int infoLogLength;
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage( std::max(infoLogLength, int(1)) );
    glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    fprintf(stdout, "%s\n", &programErrorMessage[0]);
    
    if (infoLogLength != 0){
        //error = true;
    }
    
    //Loads up all Uniforms for the shader
    GetAllUniformLocations();
}

GLuint ShaderProgram::GetUniformLocation(std::string uniformName) {
    return glGetUniformLocation(programID, uniformName.c_str());
}

void ShaderProgram::GetAllUniformLocations() {
    printf("ShaderProgram::GetAllUniformLocations() is not implemented in base class\n");
}

GLuint ShaderProgram::LoadShader(std::string file, GLint type) {
    // Create the shaders
    GLuint shaderID = glCreateShader(type);
    
    // Read the Vertex Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file, std::ios::in);
    if(shaderStream.is_open())
    {
        std::string Line = "";
        while(getline(shaderStream, Line))
            shaderCode += "\n" + Line;
        shaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Shader
    printf("Compiling shader : %s\n", file.c_str());
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer , NULL);
    glCompileShader(shaderID);
    
    // Check Vertex Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> shaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
    
    void *errorExists;
    
    errorExists = &shaderErrorMessage[0];
    if (errorExists != nullptr)
    {
        //error = true;
        fprintf(stdout, "%s\n", &shaderErrorMessage[0]);
    }
    errorExists = nullptr;
    
    return shaderID;
}

void ShaderProgram::BindAttribute(GLuint attributeIndex, std::string variable) {
    glBindAttribLocation(programID, attributeIndex, variable.c_str());
}

void ShaderProgram::LoadInt(int location, int value) {
    glUniform1i(location, value);
}

void ShaderProgram::LoadFloat(int location, float value) {
    glUniform1f(location, value);
}

void ShaderProgram::LoadVector(int location, glm::vec3 value) {
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::LoadMatrix(int location, glm::mat4 value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::LoadBoolean(int location, bool value) {
    glUniform1i(location, value);
}

void ShaderProgram::Start() {
    glUseProgram(programID);
}

void ShaderProgram::Stop() {
    glUseProgram(0);
}

void ShaderProgram::CleanUp() {
    Stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteProgram(programID);
}
