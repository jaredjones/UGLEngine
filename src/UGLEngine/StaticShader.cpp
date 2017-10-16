//
//  StaticShader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "StaticShader.h"

std::string StaticShader::VERTEX_FILE = "Resources/vert.txt";
std::string StaticShader::FRAG_FILE = "Resources/frag.txt";

void StaticShader::BindAttributes() {
    ShaderProgram::BindAttribute(0, "position");
    ShaderProgram::BindAttribute(1, "textureCoords");
    ShaderProgram::BindAttribute(2, "normal");
}

void StaticShader::GetAllUniformLocations() {
    location_transformationMatrix = ShaderProgram::GetUniformLocation("transformationMatrix");
    location_viewMatrix = ShaderProgram::GetUniformLocation("viewMatrix");
    location_projectionMatrix = ShaderProgram::GetUniformLocation("projectionMatrix");
    location_lightPosition = ShaderProgram::GetUniformLocation("lightPosition");
    location_lightColor = ShaderProgram::GetUniformLocation("lightColor");
    location_shineDamper = ShaderProgram::GetUniformLocation("shineDamper");
    location_reflectivity = ShaderProgram::GetUniformLocation("reflectivity");
}

void StaticShader::LoadTransformationMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::LoadViewMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_viewMatrix, matrix);
}

void StaticShader::LoadProjectionMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_projectionMatrix, matrix);
}

void StaticShader::LoadLight(Light *light) {
    ShaderProgram::LoadVector(location_lightPosition, light->GetPosition());
    ShaderProgram::LoadVector(location_lightColor, light->GetColor());
}

void StaticShader::LoadShineVariables(float damper, float reflectivity) {
    ShaderProgram::LoadFloat(location_shineDamper, damper);
    ShaderProgram::LoadFloat(location_reflectivity, reflectivity);
}
