//
//  EntityRenderer.cpp
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
#include "EntityRenderer.h"
#include "MasterRenderer.h"

EntityRenderer::EntityRenderer(StaticShader *shader, glm::mat4 projectionMatrix) {
    this->shader = shader;
    shader->Start();
    shader->LoadProjectionMatrix(projectionMatrix);
    shader->Stop();
}

void EntityRenderer::Render(std::unordered_map<TexturedModel *, std::list<Entity *> *> *entities) {
    for ( const auto &e : *entities )
    {
        this->PrepareTexturedModel(e.first);
        std::list<Entity *> *batch = e.second;
        for ( const auto &i : *batch)
        {
            this->PrepareInstance(i);
            glDrawElements(GL_TRIANGLES, e.first->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }
        this->UnbindTexturedModel();
    }
}

void EntityRenderer::PrepareTexturedModel(TexturedModel *model) {
    RawModel *rawModel = model->getRawModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    ModelTexture *texture = model->getModelTexture();
    if (texture->HasTransparency())
        MasterRenderer::SetCulling(false);
    shader->LoadForcedUpwardNormals(texture->HasForcedUpwardNormals());
    shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());
    
    if (texture != NULL)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    }
}

void EntityRenderer::UnbindTexturedModel() {
    MasterRenderer::SetCulling(true); // Renable culling for next model
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void EntityRenderer::PrepareInstance(Entity *entity) {
    glm::mat4 rotMatrix =           glm::rotate(glm::radians(entity->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::radians(entity->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::radians(entity->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(entity->GetScale());
    glm::mat4 translationMatrix = glm::translate(entity->GetPosition());
    
    //glm::mat4 modelMatrix =  scaleMatrix * rotMatrix  * translationMatrix;
    glm::mat4 modelMatrix =  translationMatrix * rotMatrix  * scaleMatrix;
    
    shader->LoadTransformationMatrix(modelMatrix);
}
