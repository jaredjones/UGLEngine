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
#include "Light.h"

class StaticShader: public ShaderProgram {
private:
    static std::string VERTEX_FILE;
    static std::string FRAG_FILE;
    
    GLuint location_transformationMatrix;
    GLuint location_viewMatrix;
    GLuint location_projectionMatrix;
    GLuint location_lightPosition;
    GLuint location_lightColor;
    GLuint location_shineDamper;
    GLuint location_reflectivity;
    GLuint location_forceUpwardNormals;
public:
    StaticShader(): ShaderProgram(VERTEX_FILE, FRAG_FILE) {
        LinkShader();
    }
    
    virtual ~StaticShader() {
        
    }
    
    void BindAttributes();
    void GetAllUniformLocations();
    void LoadTransformationMatrix(glm::mat4 matrix);
    void LoadViewMatrix(glm::mat4 matrix);
    void LoadProjectionMatrix(glm::mat4 matrix);
    void LoadLight(Light *light);
    void LoadShineVariables(float damper, float reflectivity);
    void LoadForcedUpwardNormals(bool enabled);
};

#endif /* StaticShader_h */

