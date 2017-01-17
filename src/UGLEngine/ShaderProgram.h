//
//  ShaderProgram.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef ShaderProgram_h
#define ShaderProgram_h

#include <string>
#include <stdio.h>

#include <glm.hpp>

class ShaderProgram {
private:
    int programID;
    int vertexShaderID;
    int fragmentShaderID;
    
    static GLuint LoadShader(std::string file, GLint type);
public:
    ShaderProgram(std::string vertexFile, std::string fragFile);
    void LinkShader();
    GLuint GetUniformLocation(std::string uniformName);
    virtual void GetAllUniformLocations() = 0;
    void Start();
    void Stop();
    void CleanUp();
    virtual void BindAttributes() = 0;
    void BindAttribute(GLuint attributeIndex, std::string variable);
    void LoadFloat(int location, float value);
    void LoadInt(int location, int value);
    void LoadVector(int location, glm::vec3 value);
    void LoadMatrix(int location, glm::mat4 value);
    void LoadBoolean(int location, bool value);
};

#endif /* ShaderProgram_h */
