//
//  StaticShader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "StaticShader.h"

std::string StaticShader::VERTEX_FILE = "/Users/jaredjones/Downloads/vert.txt";
std::string StaticShader::FRAG_FILE = "/Users/jaredjones/Downloads/frag.txt";

void StaticShader::BindAttributes() {
    ShaderProgram::BindAttribute(0, "position");
}
