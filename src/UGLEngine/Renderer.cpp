//
//  Renderer.cpp
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
#include "Renderer.h"

float FOV = 70.0f;
float NEAR_PLANE = 0.1f;
float FAR_PLANE = 1000.0f;

Renderer::Renderer(StaticShader *shader) {
    glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    projectionMatrix = glm::perspective(FOV, 800.0f/ 600.0f, NEAR_PLANE, FAR_PLANE);
    this->shader = shader;
    shader->Start();
    shader->LoadProjectionMatrix(projectionMatrix);
    shader->Stop();
}

void Renderer::Render(std::unordered_map<TexturedModel *, std::list<Entity *>> *entities) {
    for ( const auto &e : *entities )
    {
        this->PrepareTexturedModel(e.first);
        std::list<Entity *> batch = e.second;
        for ( const auto &i : batch)
        {
            this->PrepareInstance(i);
            glDrawElements(GL_TRIANGLES, e.first->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }
        this->UnbindTexturedModel();
    }
}

void Renderer::PrepareTexturedModel(TexturedModel *model) {
    RawModel *rawModel = model->getRawModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    ModelTexture *texture = model->getModelTexture();
    shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());
    
    if (texture != NULL)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    }
}

void Renderer::UnbindTexturedModel() {
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::PrepareInstance(Entity *entity) {
    glm::mat4 rotMatrix =           glm::rotate(glm::radians(entity->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::radians(entity->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::radians(entity->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(entity->GetScale());
    glm::mat4 translationMatrix = glm::translate(entity->GetPosition());
    
    //glm::mat4 modelMatrix =  scaleMatrix * rotMatrix  * translationMatrix;
    glm::mat4 modelMatrix =  translationMatrix * rotMatrix  * scaleMatrix;
    
    shader->LoadTransformationMatrix(modelMatrix);
}

/*void Renderer::Render(Entity *entity, StaticShader *shader) {
    TexturedModel *texModel = entity->GetTexturedModel();
    RawModel *rawModel = texModel->getRawModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glm::mat4 rotMatrix =           glm::rotate(glm::radians(entity->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                                    glm::rotate(glm::radians(entity->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                    glm::rotate(glm::radians(entity->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(entity->GetScale());
    glm::mat4 translationMatrix = glm::translate(entity->GetPosition());
    
    //glm::mat4 modelMatrix =  scaleMatrix * rotMatrix  * translationMatrix;
    glm::mat4 modelMatrix =  translationMatrix * rotMatrix  * scaleMatrix;
    
    shader->LoadTransformationMatrix(modelMatrix);
    
    ModelTexture *texture = texModel->getModelTexture();
    shader->LoadShineVariables(texture->GetShineDamper(), texture->GetReflectivity());
    
    if (texModel->getModelTexture() != NULL)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texModel->getModelTexture()->getTextureID());
    }
    glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}*/

void Renderer::Prepare() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
