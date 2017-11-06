//
//  TerrainRenderer.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/14/17.
//
//
#define GLM_FORCE_RADIANS

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <glm.hpp>
#include <trigonometric.hpp>
#include <gtx/transform.hpp>
#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(TerrainShader *shader, glm::mat4 projectionMatrix) {
    this->shader = shader;
    shader->Start();
    shader->LoadProjectionMatrix(projectionMatrix);
    shader->Stop();
}

void TerrainRenderer::Render(std::list<Terrain *> *terrains) {
    for ( const auto &e : *terrains)
    {
        this->PrepareTerrain(e);
        this->LoadModelMatrix(e);
        
        glDrawElements(GL_TRIANGLES, e->GetModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        
        this->UnbindTexturedModel();
    }
}

void TerrainRenderer::PrepareTerrain(Terrain *terrain) {
    RawModel *rawModel = terrain->GetModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    ModelTexture *texture = terrain->GetTexture();
    shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());
    
    if (texture != NULL)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    }
}

void TerrainRenderer::UnbindTexturedModel() {
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void TerrainRenderer::LoadModelMatrix(Terrain *terrain) {
    /*glm::mat4 rotMatrix =           glm::rotate(glm::radians(entity->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::radians(terrain->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::radians(terrain->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(terrain->GetScale());*/
    
    glm::mat4 rotMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    glm::mat4 translationMatrix = glm::translate(glm::vec3(terrain->GetX(), 0.0f, terrain->GetZ()));
    
    //glm::mat4 modelMatrix =  scaleMatrix * rotMatrix  * translationMatrix;
    glm::mat4 modelMatrix =  translationMatrix * rotMatrix  * scaleMatrix;
    
    shader->LoadTransformationMatrix(modelMatrix);
}
