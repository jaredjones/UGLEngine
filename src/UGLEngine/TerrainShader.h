//
//  TerrainShader.h
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#ifndef TerrainShader_h
#define TerrainShader_h

#include <stdio.h>
#include <string>
#include <glm.hpp>
#include "ShaderProgram.h"
#include "Light.h"

class TerrainShader: public ShaderProgram {
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
public:
    TerrainShader(): ShaderProgram(VERTEX_FILE, FRAG_FILE) {
        LinkShader();
    }
    
    virtual ~TerrainShader() {
        
    }
    
    void BindAttributes();
    void GetAllUniformLocations();
    void LoadTransformationMatrix(glm::mat4 matrix);
    void LoadViewMatrix(glm::mat4 matrix);
    void LoadProjectionMatrix(glm::mat4 matrix);
    void LoadLight(Light *light);
    void LoadShineVariables(float damper, float reflectivity);
};

#endif /* TerrainShader_h */
