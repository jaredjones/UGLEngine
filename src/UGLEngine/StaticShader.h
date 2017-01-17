//
//  StaticShader.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef StaticShader_h
#define StaticShader_h

#include <stdio.h>
#include <string>
#include <glm.hpp>
#include "ShaderProgram.h"

class StaticShader: public ShaderProgram {
private:
    static std::string VERTEX_FILE;
    static std::string FRAG_FILE;
    
    GLuint location_transformationMatrix;
public:
    StaticShader(): ShaderProgram(VERTEX_FILE, FRAG_FILE) {
        LinkShader();
    }
    
    void BindAttributes();
    void GetAllUniformLocations();
    void LoadTransformationMatrix(glm::mat4 matrix);
};

#endif /* StaticShader_h */
