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

class ShaderProgram {
private:
    int programID;
    int vertexShaderID;
    int fragmentShaderID;
    
    static GLuint LoadShader(std::string file, GLint type);
public:
    ShaderProgram(std::string vertexFile, std::string fragFile);
    void LinkShader();
    void Start();
    void Stop();
    void CleanUp();
    virtual void BindAttributes() = 0;
    void BindAttribute(GLuint attributeIndex, std::string variable);
};

#endif /* ShaderProgram_h */
