//
//  TerrainShader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "TerrainShader.h"

std::string TerrainShader::VERTEX_FILE = "Resources/terrainVert.txt";
std::string TerrainShader::FRAG_FILE = "Resources/terrainFrag.txt";

void TerrainShader::BindAttributes() {
    ShaderProgram::BindAttribute(0, "position");
    ShaderProgram::BindAttribute(1, "textureCoords");
    ShaderProgram::BindAttribute(2, "normal");
}

void TerrainShader::GetAllUniformLocations() {
    location_transformationMatrix = ShaderProgram::GetUniformLocation("transformationMatrix");
    location_viewMatrix = ShaderProgram::GetUniformLocation("viewMatrix");
    location_projectionMatrix = ShaderProgram::GetUniformLocation("projectionMatrix");
    location_lightPosition = ShaderProgram::GetUniformLocation("lightPosition");
    location_lightColor = ShaderProgram::GetUniformLocation("lightColor");
    location_shineDamper = ShaderProgram::GetUniformLocation("shineDamper");
    location_reflectivity = ShaderProgram::GetUniformLocation("reflectivity");
}

void TerrainShader::LoadTransformationMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_transformationMatrix, matrix);
}

void TerrainShader::LoadViewMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_viewMatrix, matrix);
}

void TerrainShader::LoadProjectionMatrix(glm::mat4 matrix) {
    ShaderProgram::LoadMatrix(location_projectionMatrix, matrix);
}

void TerrainShader::LoadLight(Light *light) {
    ShaderProgram::LoadVector(location_lightPosition, light->GetPosition());
    ShaderProgram::LoadVector(location_lightColor, light->GetColor());
}

void TerrainShader::LoadShineVariables(float damper, float reflectivity) {
    ShaderProgram::LoadFloat(location_shineDamper, damper);
    ShaderProgram::LoadFloat(location_reflectivity, reflectivity);
}
