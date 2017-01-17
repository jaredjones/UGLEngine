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
#include <glm.hpp>
#include <trigonometric.hpp>
#include <gtx/transform.hpp>
#include "Renderer.h"

float FOV = 70.0f;
float NEAR_PLANE = 0.1f;
float FAR_PLANE = 1000.0f;

Renderer::Renderer(StaticShader *shader) {
    glClearColor(93.0f/255.0f, 161.0f/255.0f, 219.0f/255.0f, 1.0f);
    projectionMatrix = glm::perspective(FOV, 800.0f/ 600.0f, NEAR_PLANE, FAR_PLANE);
    
    shader->Start();
    shader->LoadProjectionMatrix(projectionMatrix);
    shader->Stop();
}

void Renderer::Render(Entity *entity, StaticShader *shader) {
    TexturedModel *texModel = entity->GetTexturedModel();
    RawModel *rawModel = texModel->getRawModel();
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glm::mat4 rotMatrix =           glm::rotate(glm::radians(entity->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                                    glm::rotate(glm::radians(entity->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                    glm::rotate(glm::radians(entity->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(entity->GetScale());
    glm::mat4 translationMatrix = glm::translate(entity->GetPosition());
    
    //glm::mat4 modelMatrix =  scaleMatrix * rotMatrix  * translationMatrix;
    glm::mat4 modelMatrix =  translationMatrix * rotMatrix  * scaleMatrix;
    
    shader->LoadTransformationMatrix(modelMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texModel->getModelTexture()->getTextureID());
    glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::Prepare() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
