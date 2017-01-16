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
#include "ShaderProgram.h"

class StaticShader: public ShaderProgram {
private:
    static std::string VERTEX_FILE;
    static std::string FRAG_FILE;
public:
    StaticShader(): ShaderProgram(VERTEX_FILE, FRAG_FILE) {
        LinkShader();
    }
    
    void BindAttributes();
};

#endif /* StaticShader_h */
